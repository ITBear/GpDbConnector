#include "GpDbConnectionPgSql.hpp"
#include "GpDbQueryAsyncTaskPgSql.hpp"
#include "GpDbQueryPreparedPgSql.hpp"
#include "GpDbQueryResPgSql.hpp"

#include <GpCore2/GpTasks/GpTask.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpUtils/Other/GpRAIIonDestruct.hpp>
#include <GpCore2/GpUtils/Other/GpCallHandler2.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

namespace GPlatform {

GpDbConnectionPgSql::IsolationLevelNamesT   GpDbConnectionPgSql::sIsolationLevelNames =
{
    u8"SERIALIZABLE"_sv,
    u8"REPEATABLE READ"_sv,
    u8"READ COMMITTED"_sv,
    u8"READ UNCOMMITTED"_sv
};

GpDbConnectionPgSql::~GpDbConnectionPgSql (void) noexcept
{
    ClosePgConn();
}

void    GpDbConnectionPgSql::Close (void)
{
    ClosePgConn();
}

GpDbQueryRes::SP    GpDbConnectionPgSql::_Execute
(
    const GpDbQuery&            aQuery,
    const GpDbQueryPrepared&    aQueryPrepared,
    const size_t                aMinResultRowsCount
)
{
    //Log
    {
        //Get current task
        const auto currentTaskOpt = GpTask::SCurrentTask();

        THROW_COND_GP
        (
            currentTaskOpt.has_value(),
            u8"Ensure that the DB connection executes inside the task"_sv
        );

        const GpUUID        currentTaskGuid = currentTaskOpt.value().get().IdAsUUID();
        const std::u8string dbQueryValues   = aQuery.ValuesToStr();

        if (dbQueryValues.length() > 0)
        {
            LOG_INFO(u8"[GpDbConnectionPgSql::_Execute]: SQL '"_sv + aQuery.QueryStr() + u8"', values:\n"_sv + dbQueryValues, currentTaskGuid);
        } else
        {
            LOG_INFO(u8"[GpDbConnectionPgSql::_Execute]: SQL '"_sv + aQuery.QueryStr() + u8"'"_sv, currentTaskGuid);
        }
    }

    const GpDbConnectionMode::EnumT connMode = Mode();

    if (connMode == GpDbConnectionMode::ASYNC) [[likely]]
    {
        return ExecuteAsync(aQuery, aQueryPrepared, aMinResultRowsCount);
    } else if (connMode == GpDbConnectionMode::SYNC)
    {
        return ExecuteSync(aQuery, aQueryPrepared, aMinResultRowsCount);
    } else
    {
        THROW_GP(u8"Unknown connection mode"_sv);
    }
}

std::u8string   GpDbConnectionPgSql::StrEscape (std::u8string_view aStr) const
{
    PGconn* pgConn = static_cast<PGconn*>(iPgConn);

    if (pgConn == nullptr)
    {
        return std::u8string();
    }

    char* escapedStrPtr = PQescapeLiteral(pgConn, GpUTF::S_UTF8_To_STR(aStr).data(), aStr.size());
    std::u8string escapedStr(GpUTF::S_STR_To_UTF8(escapedStrPtr));
    PQfreemem(escapedStrPtr);

    return escapedStr;
}

bool    GpDbConnectionPgSql::Validate (void) const noexcept
{
    if (iPgConn == nullptr) [[unlikely]]
    {
        return false;
    }

    const ConnStatusType connectionStatus = PQstatus(iPgConn);
    return connectionStatus != CONNECTION_BAD;
}

void    GpDbConnectionPgSql::SetEnv (const std::vector<std::tuple<std::u8string, std::u8string>>& aValues)
{
    if (aValues.size() == 0) [[likely]]
    {
        return;
    }

    size_t sqlStrLen = 7 + (aValues.size() * 26);

    for (const auto& [a, b]: aValues)
    {
        sqlStrLen += a.length();
        sqlStrLen += b.length();
    }

    std::u8string sql;
    sql.reserve(sqlStrLen);
    sql.append(u8"select ");

    GpCallHandler2 callOnce;

    for (const auto& [a, b]: aValues)
    {
        callOnce.CallIfNonFirst([&sql](){sql.append(u8",");});
        sql.append(u8"set_config('").append(a).append(u8"', '").append(b).append(u8"', false)");
    }

    GpDbQuery               query(sql);
    GpDbQueryPreparedPgSql  queryPrepared;
    queryPrepared.Prepare(query);

    Execute(query, queryPrepared, 0);
}

void    GpDbConnectionPgSql::OnBeginTransaction (GpDbTransactionIsolation::EnumT aIsolationLevel)
{
    GpDbQuery               query(u8"BEGIN ISOLATION LEVEL "_sv + sIsolationLevelNames.at(size_t(aIsolationLevel)));
    GpDbQueryPreparedPgSql  queryPrepared;
    queryPrepared.Prepare(query);

    Execute(query, queryPrepared, 0);
}

void    GpDbConnectionPgSql::OnCommitTransaction (void)
{
    GpDbQuery               query(u8"COMMIT"_s);
    GpDbQueryPreparedPgSql  queryPrepared;
    queryPrepared.Prepare(query);

    Execute(query, queryPrepared, 0);
}

void    GpDbConnectionPgSql::OnRollbackTransaction (void)
{
    GpDbQuery               query(u8"ROLLBACK"_s);
    GpDbQueryPreparedPgSql  queryPrepared;
    queryPrepared.Prepare(query);

    Execute(query, queryPrepared, 0);
}

GpDbQueryRes::SP    GpDbConnectionPgSql::ExecuteSync
(
    const GpDbQuery&            aQuery,
    const GpDbQueryPrepared&    aQueryPrepared,
    const size_t                aMinResultRowsCount
)
{
    const GpDbQueryPreparedPgSql&   queryPrepared   = static_cast<const GpDbQueryPreparedPgSql&>(aQueryPrepared);
    std::u8string_view              queryStr        = aQuery.QueryStr();
    std::u8string                   queryZT;

    queryZT.reserve(NumOps::SAdd<size_t>(queryStr.length(), 1));
    queryZT.append(queryStr).append(u8"\0"_sv);

    PGresult* pgResult = PQexecParams
    (
        iPgConn,
        GpUTF::S_UTF8_To_STR(queryZT).data(),
        NumOps::SConvert<int>(aQuery.Values().size()),
        queryPrepared.OIDs().data(),
        queryPrepared.ValuesPtr().data(),
        queryPrepared.ValuesSize().data(),
        queryPrepared.ValuesIsBinary().data(),
        int(GpPosrgresQueryResultType::BINARY)
    );

    GpDbQueryResPgSql::SP res = MakeSP<GpDbQueryResPgSql>(pgResult);
    res.Vn().Process(aMinResultRowsCount, iPgConn);

    return res;
}

GpDbQueryRes::SP    GpDbConnectionPgSql::ExecuteAsync
(
    const GpDbQuery&            aQuery,
    const GpDbQueryPrepared&    aQueryPrepared,
    const size_t                aMinResultRowsCount
)
{
    // Create SQL request task and wait
    GpDbQueryAsyncTaskPgSql::SP queryTask = MakeSP<GpDbQueryAsyncTaskPgSql>
    (
        *this,
        aQuery,
        aQueryPrepared
    );

    GpTask::DoneFutureT::SP queryTaskDoneFuture = queryTask.Vn().GetDoneFuture();

    // Add to event poller
    const GpTaskId queryTaskTaskId = queryTask.Vn().Id();

    GpRAIIonDestruct onFinishRemoveEventPollerSubscription
    (
        [&]()
        {
            IOEventPoller()->RemoveSubscription
            (
                PQsocket(iPgConn),
                queryTaskTaskId
            );
        }
    );

    IOEventPoller()->AddSubscription
    (
        PQsocket(iPgConn),
        queryTaskTaskId,
        [](const GpTaskId aTaskId, const GpIOEventsTypes aIOEventsTypes)
        {
            GpTaskScheduler::S().MakeTaskReady(aTaskId, aIOEventsTypes);
        }
    );

    // Start query task
    GpTaskScheduler::S().NewToReady(queryTask);

    // Wait for started
    while (!queryTaskDoneFuture.Vn().WaitFor(100.0_si_ms))
    {
        // NOP
    }

    // Check result
    GpDbQueryResPgSql::SP res;

    GpTask::StartFutureT::SCheckIfReady
    (
        queryTaskDoneFuture.V(),
        [&](typename GpTaskFiber::StartFutureT::value_type& aRes)//OnSuccessFnT
        {
            LOG_INFO(u8"[GpDbConnectionPgSql::ExecuteAsync]: OK"_sv);
            res = std::move(aRes.Value<GpDbQueryResPgSql::SP>());
        },
        [](const GpException& aEx)//OnExceptionFnT
        {
            throw aEx;
        }
    );

    res.V().Process(aMinResultRowsCount, iPgConn);

    return res;
}

void    GpDbConnectionPgSql::ClosePgConn (void) noexcept
{
    PQfinish(iPgConn);
    iPgConn = nullptr;
    SetStatus(StatusTE::CLOSED);
}

}//namespace GPlatform

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
    "SERIALIZABLE"_sv,
    "REPEATABLE READ"_sv,
    "READ COMMITTED"_sv,
    "READ UNCOMMITTED"_sv
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
            "Ensure that the DB connection executes inside the task"_sv
        );

        const GpUUID        currentTaskGuid = currentTaskOpt.value().get().TaskIdAsUUID();
        const std::string   dbQueryValues   = aQuery.ValuesToStr();

        if (dbQueryValues.length() > 0)
        {
            LOG_INFO("[GpDbConnectionPgSql::_Execute]: SQL '"_sv + aQuery.QueryStr() + "', values:\n"_sv + dbQueryValues, currentTaskGuid);
        } else
        {
            LOG_INFO("[GpDbConnectionPgSql::_Execute]: SQL '"_sv + aQuery.QueryStr() + "'"_sv, currentTaskGuid);
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
        THROW_GP("Unknown connection mode"_sv);
    }
}

std::string GpDbConnectionPgSql::StrEscape (std::string_view aStr) const
{
    PGconn* pgConn = static_cast<PGconn*>(iPgConn);

    if (pgConn == nullptr)
    {
        return {};
    }

    char* escapedStrPtr = PQescapeLiteral(pgConn, std::data(aStr), std::size(aStr));
    std::string escapedStr(escapedStrPtr);
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

void    GpDbConnectionPgSql::SetEnv (const std::vector<std::tuple<std::string, std::string>>& aValues)
{
    if (aValues.empty()) [[likely]]
    {
        return;
    }

    size_t sqlStrLen = 7 + (std::size(aValues) * 26);

    for (const auto& [a, b]: aValues)
    {
        sqlStrLen += a.length();
        sqlStrLen += b.length();
    }

    std::string sql;
    sql.reserve(sqlStrLen);
    sql.append("select ");

    GpCallHandler2 callOnce;

    for (const auto& [a, b]: aValues)
    {
        callOnce.CallIfNonFirst([&sql](){sql.append(",");});
        sql.append("set_config('").append(a).append("', '").append(b).append("', false)");
    }

    GpDbQuery               query(sql);
    GpDbQueryPreparedPgSql  queryPrepared;
    queryPrepared.Prepare(query);

    Execute(query, queryPrepared, 0);
}

void    GpDbConnectionPgSql::OnBeginTransaction (GpDbTransactionIsolation::EnumT aIsolationLevel)
{
    GpDbQuery               query("BEGIN ISOLATION LEVEL "_sv + sIsolationLevelNames.at(size_t(aIsolationLevel)));
    GpDbQueryPreparedPgSql  queryPrepared;
    queryPrepared.Prepare(query);

    Execute(query, queryPrepared, 0);
}

void    GpDbConnectionPgSql::OnCommitTransaction (void)
{
    GpDbQuery               query("COMMIT"_s);
    GpDbQueryPreparedPgSql  queryPrepared;
    queryPrepared.Prepare(query);

    Execute(query, queryPrepared, 0);
}

void    GpDbConnectionPgSql::OnRollbackTransaction (void)
{
    GpDbQuery               query("ROLLBACK"_s);
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
    std::string_view                queryStr        = aQuery.QueryStr();
    std::string                     queryZT;

    queryZT.reserve(NumOps::SAdd<size_t>(queryStr.length(), 1));
    queryZT.append(queryStr).append("\0"_sv);

    PGresult* pgResult = PQexecParams
    (
        iPgConn,
        std::data(queryZT),
        NumOps::SConvert<int>(std::size(aQuery.Values())),
        std::data(queryPrepared.OIDs()),
        std::data(queryPrepared.ValuesPtr()),
        std::data(queryPrepared.ValuesSize()),
        std::data(queryPrepared.ValuesIsBinary()),
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
        aQueryPrepared,
        iIOEventPollerIdx
    );

    GpTask::DoneFutureT::SP queryTaskDoneFuture = queryTask.Vn().GetDoneFuture();

    // Add to event poller
    const GpTaskId queryTaskTaskId = queryTask.Vn().TaskId();

    GpRAIIonDestruct onFinishRemoveEventPollerSubscription
    (
        [&]()
        {
            GpIOEventPollerCatalog::S().GetByIdx(iIOEventPollerIdx)->RemoveSubscription
            (
                PQsocket(iPgConn),
                queryTaskTaskId
            );
        }
    );

    GpIOEventPollerCatalog::S().GetByIdx(iIOEventPollerIdx)->AddSubscription
    (
        PQsocket(iPgConn),
        queryTaskTaskId,
        [](const GpTaskId aTaskId, const GpIOEventPoller::SubsriberResValT aIOEventsTypes)
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
            LOG_INFO("[GpDbConnectionPgSql::ExecuteAsync]: OK"_sv);
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

}// namespace GPlatform

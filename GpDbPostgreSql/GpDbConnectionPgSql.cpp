#include "GpDbConnectionPgSql.hpp"
#include "GpDbQueryPreparedPgSql.hpp"
#include "GpDbQueryResPgSql.hpp"
#include "GpDbQueryAsyncTaskPgSql.hpp"

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

GpDbQueryRes::SP    GpDbConnectionPgSql::Execute
(
    const GpDbQuery&            aQuery,
    const GpDbQueryPrepared&    aQueryPrepared,
    const size_t                aMinResultRowsCount
)
{
    //Log
    {
        //Get current task
        GpTask* currentTask = GpTask::SCurrent();

        THROW_COND_GP
        (
            currentTask != nullptr,
            "Current task is nullptr"_sv
        );

        const GpUUID        currentTaskGuid = currentTask->Guid();
        const std::u8string dbQueryValues   = aQuery.ValuesToStr();

        if (dbQueryValues.length() > 0)
        {
            LOG_INFO(u8"[GpDbConnectionPgSql::Execute]: SQL '"_sv + aQuery.QueryStr() + u8"', values:\n"_sv + dbQueryValues, currentTaskGuid);
        } else
        {
            LOG_INFO(u8"[GpDbConnectionPgSql::Execute]: SQL '"_sv + aQuery.QueryStr() + u8"'"_sv, currentTaskGuid);
        }
    }

    switch (Mode())
    {
        case GpDbConnectionMode::SYNC:
        {
            return ExecuteSync(aQuery, aQueryPrepared, aMinResultRowsCount);
        } break;
        case GpDbConnectionMode::ASYNC:
        {
            return ExecuteAsync(aQuery, aQueryPrepared, aMinResultRowsCount);
        } break;
        default:
        {
            THROW_GP(u8"Unknown connection mode"_sv);
        }
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
    if (iPgConn == nullptr)
    {
        return false;
    }

    const ConnStatusType connectionStatus = PQstatus(iPgConn);

    return connectionStatus != CONNECTION_BAD;
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
        int(aQuery.Values().size()),
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
    const GpDbQuery&            /*aQuery*/,
    const GpDbQueryPrepared&    /*aQueryPrepared*/,
    const size_t                /*aMinResultRowsCount*/
)
{
    //TODO: reimplement
    THROW_GP_NOT_IMPLEMENTED();

    /*
    //Create SQL requesr task and wait
    GpDbQueryAsyncTaskPgSql::SP queryTask = MakeSP<GpDbQueryAsyncTaskPgSql>
    (
        "SQL async query",
        *this,
        aQuery,
        aQueryPrepared,
        aMinResultRowsCount
    );

    GpItcFuture::SP future = GpTaskScheduler::S().NewToWaitingDepend(queryTask);
    EventPoller().V().AddSubscriber
    (
        queryTask,
        queryTask.Vn().Socket().Id()
    );

    future.Vn().Wait();
    GpItcResult::SP futureRes = future.Vn().Result();

    GpDbQueryResPgSql::SP res;

    GpItcResult::SExtract<GpDbQueryResPgSql::SP>
    (
        futureRes,
        [&](GpDbQueryResPgSql::SP&& aRes)
        {
            res = std::move(aRes);
        },
        [&](std::u8string_view aError) -> GpDbQueryResPgSql::SP
        {
            THROW_DB
            (
                GpDbExceptionCode::QUERY_ERROR,
                "Failed to do query: "_sv + aError + u8"\nPG error message: " + std::u8string_view(PQerrorMessage(iPgConn))
            );
        },
        [&]()
        {
            THROW_DB
            (
                GpDbExceptionCode::QUERY_ERROR,
                "Failed to do query: DB result is null"_sv
            );
        }
    );

    res.V().Process(aMinResultRowsCount, iPgConn);

    return res;
    */
}

void    GpDbConnectionPgSql::ClosePgConn (void) noexcept
{
    PQfinish(iPgConn);
    iPgConn = nullptr;
    SetStatus(StatusTE::CLOSED);
}

}//namespace GPlatform

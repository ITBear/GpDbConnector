#include "GpDbDriverPgSql.hpp"
#include "GpDbConnectionPgSql.hpp"
#include "GpDbConnectAsyncTaskPgSql.hpp"
#include "GpDbQueryPreparedPgSql.hpp"

namespace GPlatform {

GpDbDriverPgSql::~GpDbDriverPgSql (void) noexcept
{
}

GpDbConnection::SP  GpDbDriverPgSql::NewConnection (std::string_view aConnStr) const
{
    GpDbConnection::SP connection;

    switch (Mode())
    {
        case GpDbConnectionMode::SYNC:
        {
            connection = MakeSP<GpDbConnectionPgSql>(ConnectSync(aConnStr), Mode());
        } break;
        case GpDbConnectionMode::ASYNC:
        {
            connection = MakeSP<GpDbConnectionPgSql>(ConnectAsync(aConnStr), Mode(), EventPoller());
        } break;
        default:
        {
            THROW_GP("Unknown connection mode"_sv);
        }
    }

    return connection;
}

GpDbQueryPrepared::CSP  GpDbDriverPgSql::Prepare (const GpDbQuery& aQuery) const
{
    GpDbQueryPreparedPgSql::SP queryPrepared = MakeSP<GpDbQueryPreparedPgSql>();
    queryPrepared.V().Prepare(aQuery);

    return queryPrepared;
}

PGconn* GpDbDriverPgSql::ConnectSync (std::string_view aConnStr) const
{
    const std::string connStr(aConnStr);
    PGconn* pgConn = PQconnectdb(connStr.data());

    THROW_COND_GP(pgConn != nullptr, "PQconnectdb return null"_sv);

    if (PQstatus(pgConn) == CONNECTION_BAD)
    {
        const std::string errMsg = PQerrorMessage(pgConn);
        PQfinish(pgConn);
        THROW_GP("PQconnectdb return: "_sv + errMsg);
    }

    return pgConn;
}

PGconn* GpDbDriverPgSql::ConnectAsync (std::string_view aConnStr) const
{
    //https://gist.github.com/ictlyh/6a09e8b3847199c15986d476478072e0

    THROW_COND_GP
    (
        GpTaskFiber::SIsIntoFiber(),
        "Async connection available only from inside fiber task"_sv
    );

    const std::string connStr(aConnStr);

    //Allocate
    PGconn* pgConn = PQconnectStart(connStr.data());

    GpOnThrowStackUnwindFn<std::function<void()>> onThrowStackUnwind;
    onThrowStackUnwind.Push([&](){if (pgConn != nullptr) {PQfinish(pgConn);}});

    THROW_COND_DB
    (
        pgConn != nullptr,
        GpDbExceptionCode::CONNECTION_ERROR,
        "Failed to allocate memory for PGconn"_sv
    );

    //Check status
    THROW_COND_DB
    (
        PQstatus(pgConn) != CONNECTION_BAD,
        GpDbExceptionCode::CONNECTION_ERROR,
        [&](){return "Failed to connect to DB: "_sv + std::string_view(PQerrorMessage(pgConn));}
    );

    //Set non blocking
    PQsetnonblocking(pgConn, 1);

    //Create connection task and wait
    GpDbConnectAsyncTaskPgSql::SP connectionTask = MakeSP<GpDbConnectAsyncTaskPgSql>
    (
        Name() + ": async connection to DB"_sv,
        EventPoller(),
        pgConn
    );

    GpItcFuture::SP future = GpTaskScheduler::S().NewToWaitingDepend(connectionTask);
    EventPoller().V().AddSubscriber
    (
        connectionTask,
        connectionTask.Vn().Socket().Id()
    );

    future.Vn().Wait();
    GpItcResult::SP futureRes = future.Vn().Result();

    GpDbConnectionStatus connStatus = GpItcResult::SExtract<GpDbConnectionStatus, GpDbConnectionStatus>
    (
        futureRes,
        [](GpDbConnectionStatus&& aStatus)
        {
            return std::move(aStatus);
        },
        [&](std::string_view aError) -> GpDbConnectionStatus
        {
            THROW_DB
            (
                GpDbExceptionCode::QUERY_ERROR,
                "Failed to connect to DB: "_sv + aError + "\nPG error message: " + std::string_view(PQerrorMessage(pgConn))
            );
        }
    );

    //Check result
    THROW_COND_DB
    (
        connStatus == GpDbConnectionStatus::CONNECTED,
        GpDbExceptionCode::CONNECTION_ERROR,
        [&](){return "Failed to connect to DB: "_sv + std::string_view(PQerrorMessage(pgConn));}
    );

    return pgConn;
}

}//namespace GPlatform

#include "GpDbDriverPgSql.hpp"
#include "GpDbConnectionPgSql.hpp"
#include "GpDbConnectAsyncTaskPgSql.hpp"
#include "GpDbQueryPreparedPgSql.hpp"

namespace GPlatform {

GpDbDriverPgSql::~GpDbDriverPgSql (void) noexcept
{
}

GpDbConnection::SP  GpDbDriverPgSql::NewConnection (std::u8string_view aConnStr) const
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
            THROW_GP(u8"Unknown connection mode"_sv);
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

PGconn* GpDbDriverPgSql::ConnectSync (std::u8string_view aConnStr) const
{
    const std::u8string connStr(aConnStr);
    PGconn* pgConn = PQconnectdb(GpUTF::S_UTF8_To_STR(connStr).data());

    THROW_COND_GP(pgConn != nullptr, "PQconnectdb return null"_sv);

    if (PQstatus(pgConn) == CONNECTION_BAD)
    {
        const std::u8string errMsg(GpUTF::S_STR_To_UTF8(PQerrorMessage(pgConn)));
        PQfinish(pgConn);
        THROW_GP(u8"PQconnectdb return: "_sv + errMsg);
    }

    return pgConn;
}

PGconn* GpDbDriverPgSql::ConnectAsync (std::u8string_view /*aConnStr*/) const
{
    //https://gist.github.com/ictlyh/6a09e8b3847199c15986d476478072e0

    THROW_COND_GP
    (
        GpTaskFiber::SIsIntoFiber(),
        "Async connection available only from inside fiber task"_sv
    );

    //TODO: reimplement
    THROW_GP_NOT_IMPLEMENTED();

    /*
    const std::u8string connStr(aConnStr);

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
        [&](){return u8"Failed to connect to DB: "_sv + std::u8string_view(PQerrorMessage(pgConn));}
    );

    //Set non blocking
    PQsetnonblocking(pgConn, 1);

    //Create connection task and wait
    GpDbConnectAsyncTaskPgSql::SP connectionTask = MakeSP<GpDbConnectAsyncTaskPgSql>
    (
        Name() + u8": async connection to DB"_sv,
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

    GpDbConnectionStatus connStatus;

    GpItcResult::SExtract<GpDbConnectionStatus>
    (
        futureRes,
        [&](GpDbConnectionStatus&& aStatus)
        {
            connStatus = std::move(aStatus);
        },
        [&](std::u8string_view aError) -> GpDbConnectionStatus
        {
            THROW_DB
            (
                GpDbExceptionCode::QUERY_ERROR,
                "Failed to connect to DB: "_sv + aError + u8"\nPG error message: " + std::u8string_view(PQerrorMessage(pgConn))
            );
        },
        [&]()
        {
            THROW_DB
            (
                GpDbExceptionCode::QUERY_ERROR,
                "Failed to connect to DB: result is null"_sv
            );
        }
    );

    //Check result
    THROW_COND_DB
    (
        connStatus == GpDbConnectionStatus::CONNECTED,
        GpDbExceptionCode::CONNECTION_ERROR,
        [&](){return u8"Failed to connect to DB: "_sv + std::u8string_view(PQerrorMessage(pgConn));}
    );

    return pgConn;
    */
}

}//namespace GPlatform

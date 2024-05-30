#include "GpDbDriverPgSql.hpp"
#include "GpDbConnectionPgSql.hpp"
#include "GpDbQueryPreparedPgSql.hpp"

#include <GpCore2/GpUtils/Other/GpRAIIonDestruct.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

namespace GPlatform {

GpDbDriverPgSql::~GpDbDriverPgSql (void) noexcept
{
}

GpDbConnection::SP  GpDbDriverPgSql::NewConnection (std::string_view aConnStr) const
{
    GpDbConnection::SP              connection;
    const GpDbConnectionMode::EnumT connMode = Mode();

    if (connMode == GpDbConnectionMode::ASYNC) [[likely]]
    {
        connection = MakeSP<GpDbConnectionPgSql>
        (
            ConnectAsync(aConnStr),
            Mode(),
            IOEventPollerIdx()
        );
    } else if (connMode == GpDbConnectionMode::SYNC)
    {
        connection = MakeSP<GpDbConnectionPgSql>
        (
            ConnectSync(aConnStr),
            Mode(),
            IOEventPollerIdx()
        );
    } else
    {
        THROW_GP("Unknown connection mode"_sv);
    }

    return connection;
}

GpDbQueryPrepared::CSP  GpDbDriverPgSql::Prepare (const GpDbQuery& aQuery) const
{
    GpDbQueryPreparedPgSql::SP queryPrepared = MakeSP<GpDbQueryPreparedPgSql>();
    queryPrepared.Vn().Prepare(aQuery);

    return queryPrepared;
}

PGconn* GpDbDriverPgSql::ConnectSync (std::string_view aConnStr) const
{
    const std::string connStr(aConnStr);
    PGconn* pgConn = PQconnectdb(std::data(connStr));

    THROW_COND_GP(pgConn != nullptr, "PQconnectdb return null"_sv);

    if (PQstatus(pgConn) == CONNECTION_BAD) [[unlikely]]
    {
        const std::string errMsg(PQerrorMessage(pgConn));
        PQfinish(pgConn);
        THROW_GP("PQconnectdb return: "_sv + errMsg);
    }

    return pgConn;
}

PGconn* GpDbDriverPgSql::ConnectAsync (std::string_view aConnStr) const
{
    PGconn* pgConn = ConnectSync(aConnStr);

    // Set non blocking
    PQsetnonblocking(pgConn, 1);

    return pgConn;


    /*
    // Start
    const std::string connStr(aConnStr);
    PGconn* pgConn              = PQconnectStart(std::data(connStr));
    PGconn* pgConnToAutoFinish  = pgConn;

    GpRAIIonDestruct onFinish
    (
        [&]()
        {
            if (pgConnToAutoFinish != nullptr)
            {
                PQfinish(pgConnToAutoFinish);
                pgConnToAutoFinish = nullptr;
            }
        }
    );

    THROW_COND_DB
    (
        pgConn != nullptr,
        GpDbExceptionCode::CONNECTION_ERROR,
        "PQconnectStart return null"_sv
    );

    // Check status
    THROW_COND_DB
    (
        PQstatus(pgConn) != CONNECTION_BAD,
        GpDbExceptionCode::CONNECTION_ERROR,
        [&](){return "Failed to connect to DB: "_sv + std::string_view(PQerrorMessage(pgConn));}
    );

    // Set non blocking
    PQsetnonblocking(pgConn, 1);

    // Create connection task and wait
    GpDbConnectAsyncTaskPgSql::SP connectionTask = MakeSP<GpDbConnectAsyncTaskPgSql>(pgConn);

    // Add to event poller
    const GpTaskId connectionTaskTaskId = connectionTask->Id();

    GpRAIIonDestruct onFinishRemoveEventPollerSubscription
    (
        [&]()
        {
            EventPoller().V().RemoveSubscription
            (
                PQsocket(pgConn),
                connectionTaskTaskId
            );
        }
    );

    EventPoller().V().AddSubscription
    (
        PQsocket(pgConn),
        connectionTaskTaskId,
        [connectionTaskTaskId](const GpIOEventsTypes aIOEventsTypes)
        {
            GpTaskScheduler::S().MakeTaskReady(connectionTaskTaskId, aIOEventsTypes);
        }
    );

    // Get done future
    GpTask::DoneFutureT::SP connectionTaskDoneFuture = connectionTask.Vn().GetDoneFuture();

    GpTaskScheduler::S().NewToReady(connectionTask);

    // Wait for started
    while (!connectionTaskDoneFuture.Vn().WaitFor(100.0_si_ms))
    {
        // NOP
    }

    // Check result
    GpDbConnectionStatus connStatusRes;

    GpTask::StartFutureT::SCheckIfReady
    (
        connectionTaskDoneFuture.V(),
        [&](typename GpTaskFiber::StartFutureT::value_type& aRes)//OnSuccessFnT
        {
            connStatusRes = aRes.Value<GpDbConnectionStatus>();
            LOG_INFO("[GpDbDriverPgSql::ConnectAsync]: "_sv + connStatusRes.ToString());
        },
        [](void)//OnEmptyFnT
        {
            LOG_ERROR("[GpDbDriverPgSql::ConnectAsync]: empty result"_sv);
        },
        [](const GpException& aEx)//OnExceptionFnT
        {
            throw aEx;
        }
    );

    //Check result
    THROW_COND_DB
    (
        connStatusRes == GpDbConnectionStatus::CONNECTED,
        GpDbExceptionCode::CONNECTION_ERROR,
        [&](){return "Failed to connect to DB: "_sv + std::string_view(PQerrorMessage(pgConn));}
    );

    pgConnToAutoFinish = nullptr;

    return pgConn;*/
}

}// namespace GPlatform

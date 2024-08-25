#include <GpDbConnector/GpDbPostgreSql/GpDbConnectionPgSql.hpp>
#include <GpCore2/GpUtils/Debugging/GpDebugging.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/ITC/GpItcSharedFutureUtils.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

namespace GPlatform {

const GpDbConnectionPgSql::IsolationLevelNamesT GpDbConnectionPgSql::sIsolationLevelNames =
{
    "SERIALIZABLE"_sv,
    "REPEATABLE READ"_sv,
    "READ COMMITTED"_sv,
    "READ UNCOMMITTED"_sv
};

GpDbConnectionPgSql::GpDbConnectionPgSql
(
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    const milliseconds_t        aConnectTimeout,
    std::string                 aServerHost,
    const u_int_16              aServerPort,
    std::string                 aUserName,
    std::string                 aPassword,
    std::string                 aDatabase
):
GpDbConnection{},
iIOEventPollerIdx{aIOEventPollerIdx},
iConnectTimeout  {aConnectTimeout},
iServerHost      {std::move(aServerHost)},
iServerPort      {aServerPort},
iUserName        {std::move(aUserName)},
iPassword        {std::move(aPassword)},
iDatabase        {std::move(aDatabase)}
{
}

GpDbConnectionPgSql::~GpDbConnectionPgSql (void) noexcept
{
    _Close();
}

void    GpDbConnectionPgSql::Close (void)
{
    _Close();
}

GpDbQueryRes::SP    GpDbConnectionPgSql::Execute
(
    const GpDbQuery&    aQuery,
    const size_t        aMinResultRowsCount
)
{   
    return iConnectionTaskSP->Execute(aQuery, aMinResultRowsCount);
}

bool    GpDbConnectionPgSql::IsConnected (void) const noexcept
{
    if (iConnectionTaskSP.IsNULL()) [[unlikely]]
    {
        return false;
    }

    return iConnectionTaskSP.Vn().IsConnected();
}

void    GpDbConnectionPgSql::TryConnectAndWaitFor (void)
{
    // Close old connection
    _Close();

    // Create new GpDbConnectionTaskPgSql
    iConnectionTaskSP = MakeSP<GpDbConnectionTaskPgSql>
    (
        iIOEventPollerIdx,
        iConnectTimeout,
        iServerHost,
        iServerPort,
        iUserName,
        iPassword,
        iDatabase
    );

    // Add task to ready
    GpTask::DoneFutureT::SP                             doneFutureSP        = iConnectionTaskSP.Vn().GetDoneFuture();
    GpDbConnectionTaskPgSql::ConnectedToDbFutureT::SP   connectedToDbFuture = iConnectionTaskSP.Vn().GetConnectedToDbFuture();

    GpTaskScheduler::S().NewToReady(iConnectionTaskSP);

    // Wait for start
    GpItcSharedFutureUtils::SWaitForInf
    (
        iConnectionTaskSP.Vn().GetStartFuture().V(),
        [&](typename GpTaskFiber::StartFutureT::value_type&)// OnSuccessFnT
        {
            // NOP
        },
        [&](const GpException& aEx)// OnExceptionFnT
        {
            LOG_ERROR
            (
                fmt::format
                (
                    "[GpDbConnectionPgSql::TryConnectAndWaitFor]: Failed to connect to PostgreSQL {}:{}, database '{}'",
                    iServerHost,
                    iServerPort,
                    iDatabase
                )
            );

            throw aEx;
        },
        100.0_si_ms
    );

    // Wait for connect or done
    {
        auto onConnectSuccessFn = [&](typename GpDbConnectionTaskPgSql::ConnectedToDbFutureT::value_type&)// OnSuccessFnT
        {
            LOG_INFO
            (
                fmt::format
                (
                    "[GpDbConnectionPgSql::TryConnectAndWaitFor]: Connected to PostgreSQL {}:{}, database '{}'",
                    iServerHost,
                    iServerPort,
                    iDatabase
                )
            );
        };

        auto onConnectExceptionFn = [&](const GpException& aEx)// OnExceptionFnT
        {
            LOG_ERROR
            (
                fmt::format
                (
                    "[GpDbConnectionPgSql::TryConnectAndWaitFor]: Failed to connect to PostgreSQL {}:{}, database '{}'",
                    iServerHost,
                    iServerPort,
                    iDatabase
                )
            );

            throw aEx;
        };

        auto onConnectFuturePack = std::tuple<GpDbConnectionTaskPgSql::ConnectedToDbFutureT&, decltype(onConnectSuccessFn), decltype(onConnectExceptionFn)>
        {
            connectedToDbFuture.Vn(),
            onConnectSuccessFn,
            onConnectExceptionFn
        };

        auto onDoneSuccessFn = [&](typename GpTask::DoneFutureT::value_type&)// OnSuccessFnT
        {
            THROW_GP
            (
                fmt::format
                (
                    "[GpDbConnectionPgSql::TryConnectAndWaitFor]: Connection task return done future while connecting to PostgreSQL {}:{}, database '{}'",
                    iServerHost,
                    iServerPort,
                    iDatabase
                )
            );
        };

        auto onDoneExceptionFn = [&](const GpException& aEx)// OnExceptionFnT
        {
            throw aEx;
        };

        auto onDoneFuturePack = std::tuple<GpTask::DoneFutureT&, decltype(onDoneSuccessFn), decltype(onDoneExceptionFn)>
        {
            doneFutureSP.Vn(),
            onDoneSuccessFn,
            onDoneExceptionFn
        };

        GpItcSharedFutureUtils::SWaitForInfAny
        (
            100.0_si_ms,
            onConnectFuturePack,
            onDoneFuturePack
        );
    }
}

void    GpDbConnectionPgSql::OnBeginTransaction ([[maybe_unused]] GpDbTransactionIsolation::EnumT aIsolationLevel)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //GpDbQuery             query("BEGIN ISOLATION LEVEL "_sv + sIsolationLevelNames.at(size_t(aIsolationLevel)));
    //GpDbQueryPreparedPgSql    queryPrepared;
    //queryPrepared.Prepare(query);

    //Execute(query, queryPrepared, 0);
}

void    GpDbConnectionPgSql::OnCommitTransaction (void)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //GpDbQuery             query("COMMIT"_sv);
    //GpDbQueryPreparedPgSql    queryPrepared;
    //queryPrepared.Prepare(query);

    //Execute(query, queryPrepared, 0);
}

void    GpDbConnectionPgSql::OnRollbackTransaction (void)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //GpDbQuery             query("ROLLBACK"_sv);
    //GpDbQueryPreparedPgSql    queryPrepared;
    //queryPrepared.Prepare(query);

    //Execute(query, queryPrepared, 0);
}

void    GpDbConnectionPgSql::_Close (void) noexcept
{
    if (iConnectionTaskSP.IsNULL())
    {
        return;
    }

    try
    {
        iConnectionTaskSP.Vn().RequestAndWaitForStop();
        iConnectionTaskSP.Clear();
    } catch (const GpException& e)
    {
        LOG_EXCEPTION("GpDbConnectionPgSql::_Close", e);
    } catch (const std::exception& e)
    {
        LOG_EXCEPTION("GpDbConnectionPgSql::_Close", GpException{e.what()});
    } catch (...)
    {
        LOG_EXCEPTION("GpDbConnectionPgSql::_Close", GpException{"[DnsClientUdpTask::OnStop]: unknown exception"_sv});
    }
}

}// namespace GPlatform

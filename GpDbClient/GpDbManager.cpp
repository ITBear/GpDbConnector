#include <GpDbConnector/GpDbClient/GpDbManager.hpp>
#include <GpDbConnector/GpDbClient/GpDbDriver.hpp>
#include <GpDbConnector/GpDbClient/GpDbConnection.hpp>
#include <GpCore2/GpTasks/ITC/GpItcSharedFutureUtils.hpp>

namespace GPlatform {

GpDbManager::GpDbManager
(
    GpDbDriver::CSP             aDriver,
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    const milliseconds_t        aConnectTimeout,
    std::string                 aServerHost,
    const u_int_16              aServerPort,
    std::string                 aUserName,
    std::string                 aPassword,
    std::string                 aDatabase
) noexcept:
GpElementsPool{},
iDriver          {std::move(aDriver)},
iIOEventPollerIdx{std::move(aIOEventPollerIdx)},
iConnectTimeout  {std::move(aConnectTimeout)},
iServerHost      {std::move(aServerHost)},
iServerPort      {std::move(aServerPort)},
iUserName        {std::move(aUserName)},
iPassword        {std::move(aPassword)},
iDatabase        {std::move(aDatabase)}
{
}

GpDbManager::~GpDbManager (void) noexcept
{
}

//GpDbQueryPrepared::CSP    GpDbManager::Prepare (const GpDbQuery& aQuery) const
//{
//  return iDriver.V().Prepare(aQuery);
//}

void    GpDbManager::PreInit (const size_t /*aCount*/)
{
}

GpDbConnection::SP  GpDbManager::NewElement (void)
{
    return iDriver.V().NewConnection
    (
        iIOEventPollerIdx,
        iConnectTimeout,
        iServerHost,
        iServerPort,
        iUserName,
        iPassword,
        iDatabase
    );
}

void    GpDbManager::OnClear (void) noexcept
{
    // NOP
}

bool    GpDbManager::Validate (GpDbConnection::SP& aDbConnection) noexcept
{
    const GpDbConnection& dbConnection = aDbConnection.Vn();
    return dbConnection.IsConnected();
}

void    GpDbManager::OnAcquire (GpDbConnection::SP& aDbConnection)
{
    GpDbConnection& dbConnection = aDbConnection.Vn();

    if (dbConnection.IsConnected()) [[likely]]
    {
        return;
    }

    // Try to connect
    dbConnection.TryConnectAndWaitFor();
}

GpDbManager::ReleaseAct GpDbManager::OnRelease (GpDbConnection::SP& aDbConnection)
{
    // Check if there are NO tasks waiting for connections
    if (iConnWaitToAcquirePromises.empty()) [[likely]]
    {
        return ReleaseAct::PUSH_TO_ELEMENTS;
    }

    // Notify waiting task
    ConnectItcPromiseT promise(std::move(iConnWaitToAcquirePromises.front()));
    iConnWaitToAcquirePromises.pop();

    promise.Fulfill(std::move(aDbConnection));

    return ReleaseAct::ACQUIRED;
}

GpDbConnection::C::Opt::SP  GpDbManager::OnAcquireNoElementsLeft (void)
{
    // Wait for Release any connection to pool
    ConnectItcPromiseT      promise;
    ConnectItcFutureT::SP   future = promise.Future();
    iConnWaitToAcquirePromises.push(std::move(promise));

    // Wait for future
    std::optional<GpDbConnection::SP> res;

    // Check result
    GpItcSharedFutureUtils::SWaitForInf
    (
        future.V(),
        [&](GpDbConnection::SP& aConnection)
        {
            res = std::move(aConnection);
        },
        [](const GpException& aEx)
        {
            throw aEx;
        },
        250.0_si_ms
    );

    return res;
}

}// namespace GPlatform

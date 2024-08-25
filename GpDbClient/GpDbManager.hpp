#pragma once

#include <GpCore2/GpUtils/Types/Containers/GpElementsPool.hpp>
#include <GpCore2/GpTasks/ITC/GpItcSharedPromise.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>
#include <GpDbConnector/GpDbClient/GpDbDriver.hpp>
#include <GpDbConnector/GpDbClient/Query/GpDbQueryPrepared.hpp>
#include <GpDbConnector/GpDbClient/Query/GpDbQuery.hpp>
#include <GpDbConnector/GpDbClient/GpDbConnection.hpp>

namespace GPlatform {

class GpDbManagerCfgDesc;

class GP_DB_CLIENT_API GpDbManager final: public GpElementsPool<GpDbConnection::SP>
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbManager)
    CLASS_DD(GpDbManager)
    TAG_SET(THREAD_SAFE)

    using ConnectItcPromiseT    = GpItcSharedPromise<GpDbConnection::SP>;
    using ConnectItcFutureT     = GpItcSharedFuture<GpDbConnection::SP>;

public:
                                        GpDbManager             (GpDbDriver::CSP    aDriver,
                                                                 GpIOEventPollerIdx aIOEventPollerIdx,
                                                                 milliseconds_t     aConnectTimeout,
                                                                 std::string        aServerHost,
                                                                 u_int_16           aServerPort,
                                                                 std::string        aUserName,
                                                                 std::string        aPassword,
                                                                 std::string        aDatabase) noexcept;
    virtual                             ~GpDbManager            (void) noexcept override final;

    //GpDbQueryPrepared::CSP            Prepare                 (const GpDbQuery& aQuery) const;

protected:
    virtual void                        PreInit                 (size_t aCount) override final;
    virtual GpDbConnection::SP          NewElement              (void) override final;
    virtual void                        OnClear                 (void) noexcept override final;
    virtual bool                        Validate                (GpDbConnection::SP& aConnection) noexcept override final;

    virtual void                        OnAcquire               (GpDbConnection::SP& aDbConnection) override final;
    virtual ReleaseAct                  OnRelease               (GpDbConnection::SP& aDbConnection) override final;
    virtual GpDbConnection::C::Opt::SP  OnAcquireNoElementsLeft (void) override final;

private:
    const GpDbDriver::CSP               iDriver;
    const GpIOEventPollerIdx            iIOEventPollerIdx;
    const milliseconds_t                iConnectTimeout;
    const std::string                   iServerHost;
    const u_int_16                      iServerPort;
    const std::string                   iUserName;
    const std::string                   iPassword;
    const std::string                   iDatabase;

    std::queue<ConnectItcPromiseT>      iConnWaitToAcquirePromises;
};

}// GPlatform

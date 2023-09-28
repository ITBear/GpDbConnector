#pragma once

#include "GpDbConnectionMode.hpp"
#include "Query/GpDbQueryPrepared.hpp"
#include "Query/GpDbQuery.hpp"
#include "../../GpCore2/GpUtils/Types/Containers/GpElementsPool.hpp"
#include "../../GpCore2/GpTasks/ITC/GpItcSharedPromise.hpp"

namespace GPlatform {

class GpDbDriver;
class GpDbConnection;
class GpDbManagerCfgDesc;

class GP_DB_CLIENT_API GpDbManager final: public GpElementsPool<GpSP<GpDbConnection>>
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbManager)
    CLASS_DD(GpDbManager)
    TAG_SET(THREAD_SAFE)

    using ConnectItcPromiseT    = GpItcSharedPromise<GpSP<GpDbConnection>>;
    using ConnectItcFutureT     = GpItcSharedFuture<GpSP<GpDbConnection>>;

public:
                                                GpDbManager             (GpSP<GpDbDriver>                   aDriver,
                                                                         std::u8string                      aConnectionStr,
                                                                         const GpDbConnectionMode::EnumT    aMode) noexcept;
    virtual                                     ~GpDbManager            (void) noexcept override final;

    GpDbQueryPrepared::CSP                      Prepare                 (const GpDbQuery& aQuery) const;

protected:
    virtual void                                PreInit                 (const size_t aCount) override final;
    virtual GpSP<GpDbConnection>                NewElement              (GpSpinlock& aLocked) override final;
    virtual void                                OnClear                 (void) noexcept override final;
    virtual bool                                Validate                (GpSP<GpDbConnection>& aConnection) noexcept override final;

    virtual void                                OnAcquire               (GpSP<GpDbConnection>& aDbConnection,
                                                                         GpSpinlock& aLocked) override final;
    virtual ReleaseAct                          OnRelease               (GpSP<GpDbConnection>& aDbConnection,
                                                                         GpSpinlock& aLocked) override final;
    virtual std::optional<GpSP<GpDbConnection>> OnAcquireNoElementsLeft (GpSpinlock& aLocked) override final;

private:
    GpSP<GpDbDriver>                            iDriver;
    const std::u8string                         iConnStr;
    const GpDbConnectionMode::EnumT             iMode;
    std::queue<ConnectItcPromiseT>              iConnWaitPromises;
};

}// GPlatform

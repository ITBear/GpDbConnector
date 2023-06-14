#include "GpDbManager.hpp"
#include "GpDbDriver.hpp"
#include "GpDbConnection.hpp"
#include "GpDbManagerCfgDesc.hpp"

namespace GPlatform {

GpDbManager::GpDbManager
(
    GpSP<GpDbDriver>                aDriver,
    std::u8string                   aConnectionStr,
    const GpDbConnectionMode::EnumT aMode
) noexcept:
iDriver(std::move(aDriver)),
iConnStr(std::move(aConnectionStr)),
iMode(aMode)
{
}

GpDbManager::~GpDbManager (void) noexcept
{
}

GpDbQueryPrepared::CSP  GpDbManager::Prepare (const GpDbQuery& aQuery) const
{
    return iDriver.V().Prepare(aQuery);
}

void    GpDbManager::PreInit (const size_t /*aCount*/)
{
}

GpDbConnection::SP  GpDbManager::NewElement (GpSpinlock& aLocked)
{
    GpUnlockGuard unlock(aLocked);

    GpDbConnection::SP conn = iDriver.Vn().NewConnection(iConnStr);
    return conn;
}

void    GpDbManager::OnClear (void) noexcept
{
    //NOP
}

bool    GpDbManager::Validate (GpSP<GpDbConnection> aConnection) noexcept
{
    return aConnection.V().Validate();
}

void    GpDbManager::OnAcquire
(
    value_type& /*aValue*/,
    GpSpinlock& /*aLocked*/
)
{
    //NOP
}

GpDbManager::ReleaseAct GpDbManager::OnRelease
(
    value_type& aValue,
    GpSpinlock& /*aLocked*/
)
{
    if (iMode == GpDbConnectionMode::SYNC)
    {
        return ReleaseAct::PUSH_TO_ELEMENTS;
    }

    //Mode is ASYNC, check if there are NO tasks waiting for connections
    if (iConnWaitPromises.size() == 0)
    {
        return ReleaseAct::PUSH_TO_ELEMENTS;
    }

    //Notify waiting task
    ConnectItcPromiseT promise(std::move(iConnWaitPromises.front()));
    iConnWaitPromises.pop();

    promise.Complete(MakeSP<ConnectItcResultT>(value_type(aValue)));

    return ReleaseAct::ACQUIRED;
}

std::optional<GpDbManager::value_type>  GpDbManager::OnAcquireNoElementsLeft (GpSpinlock& /*aLocked*/)
{
    if (iMode == GpDbConnectionMode::SYNC)
    {
        return std::nullopt;
    }

    //TODO: reimplement
    THROW_GP_NOT_IMPLEMENTED();

    /*
    //Mode is ASYNC, wait for Release next connection
    GpItcPromise    promise;
    GpItcFuture::SP future  = promise.Future();
    iConnWaitPromises.push(std::move(promise));

    GpUnlockGuard unlock(aLocked);

    future.Vn().Wait();
    GpItcResult::SP futureRes = future.Vn().Result();

    std::optional<value_type> res;

    GpItcResult::SExtract<value_type>
    (
        futureRes,
        [&](value_type&& aValue)
        {
            res = std::move(aValue);
        },
        [](std::u8string_view aError)
        {
            LOG_ERROR(aError, GpTask::SCurrentUID());
        },
        []()
        {
        }
    );

    return res;
    */
}

}//namespace GPlatform

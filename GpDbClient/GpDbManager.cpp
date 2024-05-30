#include "GpDbManager.hpp"
#include "GpDbDriver.hpp"
#include "GpDbConnection.hpp"

namespace GPlatform {

GpDbManager::GpDbManager
(
    GpSP<GpDbDriver>                aDriver,
    std::string                     aConnectionStr,
    const GpDbConnectionMode::EnumT aMode
) noexcept:
iDriver (std::move(aDriver)),
iConnStr(std::move(aConnectionStr)),
iMode   (aMode)
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

GpDbConnection::SP  GpDbManager::NewElement (void)
{
    //GpUnlockGuard unlock(aLocked);

    return iDriver.V().NewConnection(iConnStr);
}

void    GpDbManager::OnClear (void) noexcept
{
    // NOP
}

bool    GpDbManager::Validate (GpSP<GpDbConnection>& aConnection) noexcept
{
    return aConnection.V().Validate();
}

void    GpDbManager::OnAcquire
(
    GpSP<GpDbConnection>&   /*aDbConnection*/
)
{
    // NOP
}

GpDbManager::ReleaseAct GpDbManager::OnRelease
(
    GpSP<GpDbConnection>&   /*aDbConnection*/
)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    /*if (iMode == GpDbConnectionMode::ASYNC) [[likely]]
    {
        // Check if there are NO tasks waiting for connections
        if (iConnWaitPromises.empty()) [[likely]]
        {
            return ReleaseAct::PUSH_TO_ELEMENTS;
        }

        // Notify waiting task
        ConnectItcPromiseT promise(std::move(iConnWaitPromises.front()));
        iConnWaitPromises.pop();

        //GpUnlockGuard unlock(aLocked);

        promise.Fulfill(aDbConnection);

        return ReleaseAct::ACQUIRED;
    } else if (iMode == GpDbConnectionMode::SYNC)
    {
        return ReleaseAct::PUSH_TO_ELEMENTS;
    } else
    {
        THROW_GP("Unknown DB connection mode"_sv);
    }*/
}

std::optional<GpSP<GpDbConnection>> GpDbManager::OnAcquireNoElementsLeft (void)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    /*if (iMode == GpDbConnectionMode::ASYNC) [[likely]]
    {
        // Mode is ASYNC, wait for Release next connection
        ConnectItcPromiseT      promise;
        ConnectItcFutureT::SP   future = promise.Future();
        iConnWaitPromises.push(std::move(promise));

        //GpUnlockGuard unlock(aLocked);

        // Wait for future
        while (!future.Vn().WaitFor(250.0_si_ms))
        {
            // NOP
        }

        std::optional<GpSP<GpDbConnection>> res;

        // Check result
        ConnectItcFutureT::SCheckIfReady
        (
            future.V(),
            [&](GpSP<GpDbConnection>& aConnection)
            {
                res = std::move(aConnection);
            },
            [](void)
            {
                // NOP
            },
            [](const GpException& aEx)
            {
                throw aEx;
            }
        );

        return res;
    } else if (iMode == GpDbConnectionMode::SYNC)
    {
        return std::nullopt;
    } else
    {
        THROW_GP("Unknown DB connection mode"_sv);
    }*/
}

}// namespace GPlatform

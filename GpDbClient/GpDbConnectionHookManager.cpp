#include "GpDbConnectionHookManager.hpp"
#include <shared_mutex>
#include <mutex>

namespace GPlatform {

GpDbConnectionHookManager   GpDbConnectionHookManager::sInstance;

void    GpDbConnectionHookManager::AddGlobalHook
(
    HookFnT&&   aFn,
    const Mode  aMode
)
{
    auto& [rwLock, hooks] = iGlobalHooks[size_t(aMode)];

    {
        std::scoped_lock lock(rwLock);
        hooks.emplace_back(std::move(aFn));
    }
}

void    GpDbConnectionHookManager::OnExecute
(
    GpDbConnection& aConn,
    const Mode      aMode
)
{
    auto& [rwLock, hooks] = iGlobalHooks[size_t(aMode)];

    {
        std::shared_lock lock(rwLock);

        for (auto& hookFn: hooks)
        {
            if (hookFn) [[likely]]
            {
                hookFn(aConn);
            }
        }
    }
}

}//namespace GPlatform

#pragma once

#include "GpDbClient_global.hpp"

#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>
#include <functional>
#include <vector>

namespace GPlatform {

class GpDbConnection;

class GP_DB_CLIENT_API GpDbConnectionHookManager
{
public:
    TAG_SET(THREAD_SAFE)

    enum class Mode
    {
        BEFORE_EXECUTE,
        AFTER_EXECUTE
    };

    using HookFnT       = std::function<void(GpDbConnection& aConn)>;
    using HookFnVecT    = std::vector<HookFnT>;
    using HooksByModeT  = std::array<std::tuple<GpSpinLockRW, HookFnVecT>, 2>;

private:
                                        GpDbConnectionHookManager   (void) = default;

public:
                                        ~GpDbConnectionHookManager  (void) = default;

    static GpDbConnectionHookManager&   S                           (void) noexcept {return sInstance;}

    void                                AddGlobalHook               (HookFnT&&  aFn,
                                                                     const Mode aMode);
    void                                OnExecute                   (GpDbConnection&    aConn,
                                                                     const Mode         aMode);

private:
    HooksByModeT                        iGlobalHooks;
    static GpDbConnectionHookManager    sInstance;
};

}// namespace GPlatform

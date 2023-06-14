#include "GpDbManagerCatalog.hpp"
#include "GpDbManager.hpp"
#include "GpDbDriverFactory.hpp"
#include "GpDbDriver.hpp"
#include "../../GpNetwork/GpNetworkCore/IO/Events/GpIOEventPollerCatalog.hpp"

namespace GPlatform {

GpDbManagerCatalog  GpDbManagerCatalog::sInstance;

GpDbManagerCatalog::GpDbManagerCatalog (void) noexcept
{
}

GpDbManagerCatalog::~GpDbManagerCatalog (void) noexcept
{
    iManagers.Clear();
}

void    GpDbManagerCatalog::Init
(
    const GpDbManagerCfgDesc::C::Vec::SP&   aCfgDescs,
    const GpDbDriverCatalog&                aDbDriverCatalog
)
{
    for (const GpDbManagerCfgDesc::SP& cfgDescSP: aCfgDescs)
    {
        const GpDbManagerCfgDesc&   cfgDesc         = cfgDescSP.V();
        const GpDbDriverFactory&    driverFactory   = aDbDriverCatalog.Find(cfgDesc.driver_name);
        GpDbDriver::SP              driverSP        = driverFactory.NewInstance
        (
            cfgDesc.mode.Value(),
            GpIOEventPollerCatalog::S().Find(cfgDesc.event_poller)
        );

        GpDbManager::SP dbManager = MakeSP<GpDbManager>
        (
            driverSP,
            cfgDesc.connection_str,
            cfgDesc.mode.Value()
        );

        dbManager.Vn().Init(0, cfgDesc.max_conn_pool_size);

        Add(dbManager, cfgDesc.aliases);
    }
}

void    GpDbManagerCatalog::Clear (void)
{
    iManagers.Clear();
}

void    GpDbManagerCatalog::Add
(
    GpDbManager::SP                     aManager,
    const std::vector<std::u8string>&   aAliases
)
{
    for (const auto& alias: aAliases)
    {
        iManagers.Set(alias, aManager);
    }
}

GpDbManager&    GpDbManagerCatalog::Find (std::u8string_view aName)
{
    auto res = iManagers.GetOpt(aName);

    THROW_COND_GP
    (
        res.has_value(),
        [&](){return u8"DB manager not found by name '"_sv + aName + u8"'"_sv;}
    );

    return res.value().get().V();
}

}//namespace GPlatform

#include "GpDbManagerCatalog.hpp"
#include "GpDbManager.hpp"
#include "GpDbDriverFactory.hpp"
#include "GpDbDriver.hpp"

namespace GPlatform {

GpDbManagerCatalog::GpDbManagerCatalog (void) noexcept
{
}

GpDbManagerCatalog::~GpDbManagerCatalog (void) noexcept
{
    iManagers.Clear();
}

GpDbManagerCatalog& GpDbManagerCatalog::S (void) noexcept
{
    static GpDbManagerCatalog sDbGlobal;
    return sDbGlobal;
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
        const GpDbDriverFactory&    driverFactory   = aDbDriverCatalog.Find(cfgDesc.DriverName());
        GpDbDriver::SP              driverSP        = driverFactory.NewInstance
        (
            cfgDesc.Mode(),
            GpIOEventPollerCatalog::S().Find(cfgDesc.EventPoller())
        );

        GpDbManager::SP dbManager = MakeSP<GpDbManager>
        (
            driverSP,
            std::string(cfgDesc.ConnectionStr()),
            cfgDesc.Mode()
        );

        dbManager.Vn().Init(0, cfgDesc.MaxConnPoolSize());

        Add(dbManager, cfgDesc.Aliases());
    }
}

void    GpDbManagerCatalog::Clear (void)
{
    iManagers.Clear();
}

void    GpDbManagerCatalog::Add
(
    GpDbManager::SP                 aManager,
    const std::vector<std::string>& aAliases
)
{
    for (const auto& alias: aAliases)
    {
        iManagers.Register(alias, aManager);
    }
}

GpDbManager&    GpDbManagerCatalog::Find (std::string_view aName)
{
    auto res = iManagers.FindOpt(aName);

    THROW_COND_GP
    (
        res.has_value(),
        [&](){return "DB manager not found by name '"_sv + aName + "'"_sv;}
    );

    return res.value().get().V();
}

}//namespace GPlatform

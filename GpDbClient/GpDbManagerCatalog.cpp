#include <GpDbConnector/GpDbClient/GpDbManagerCatalog.hpp>
#include <GpDbConnector/GpDbClient/GpDbManager.hpp>
#include <GpDbConnector/GpDbClient/GpDbDriverFactory.hpp>
#include <GpDbConnector/GpDbClient/GpDbDriver.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>

namespace GPlatform {

GpDbManagerCatalog  GpDbManagerCatalog::sInstance;

GpDbManagerCatalog::GpDbManagerCatalog (void) noexcept
{
}

GpDbManagerCatalog::~GpDbManagerCatalog (void) noexcept
{
    iManagers.Clear();
}

void    GpDbManagerCatalog::Start
(
    const GpDbManagerCfgDesc::C::Vec::SP&   aCfgDescs,
    const GpDbDriverCatalog&                aDbDriverCatalog
)
{
    for (const GpDbManagerCfgDesc::SP& cfgDescSP: aCfgDescs)
    {
        const GpDbManagerCfgDesc&   cfgDesc         = cfgDescSP.V();
        const GpIOEventPollerIdx    eventPollerIdx  = GpIOEventPollerCatalog::S().IdxByName(cfgDesc.event_poller_name);

        GpDbDriverFactory::SP   driverFactorySP = aDbDriverCatalog.Find(cfgDesc.driver_name);
        GpDbDriver::SP          driverSP        = driverFactorySP->NewInstance
        (
            cfgDesc.mode,
            eventPollerIdx
        );

        GpDbManager::SP dbManager = MakeSP<GpDbManager>
        (
            driverSP,
            cfgDesc.connection_str,
            cfgDesc.mode
        );

        dbManager.Vn().Init(0, cfgDesc.max_conn_pool_size);

        Add(dbManager, cfgDesc.aliases);
    }
}

void    GpDbManagerCatalog::StopAndClear (void)
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
        iManagers.SetOrUpdate(alias, aManager);
    }
}

GpDbManager&    GpDbManagerCatalog::Find (std::string_view aName)
{
    auto res = iManagers.GetOpt(aName);

    THROW_COND_GP
    (
        res.has_value(),
        [aName]()
        {
            return fmt::format
            (
                "DB manager not found by name '{}'",
                aName
            );
        }
    );

    return res.value().Vn();
}

}// namespace GPlatform

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
    // Read config
    for (const GpDbManagerCfgDesc::SP& cfgDescSP: aCfgDescs)
    {
        const GpDbManagerCfgDesc&   dbManagerCfgDesc    = cfgDescSP.V();
        const GpIOEventPollerIdx    eventPollerIdx      = GpIOEventPollerCatalog::S().IdxByName(dbManagerCfgDesc.event_poller_name);
        GpDbDriverFactory::SP       driverFactorySP     = aDbDriverCatalog.Find(dbManagerCfgDesc.driver_name);
        GpDbDriver::CSP             driverCSP           = driverFactorySP->NewInstance();

        GpDbManager::SP dbManager = MakeSP<GpDbManager>
        (
            std::move(driverCSP),
            eventPollerIdx,
            dbManagerCfgDesc.connect_timeout,
            dbManagerCfgDesc.db_host,
            dbManagerCfgDesc.db_port,
            dbManagerCfgDesc.db_user_name,
            dbManagerCfgDesc.db_password,
            dbManagerCfgDesc.db_name
        );

        dbManager.Vn().Init(0, dbManagerCfgDesc.max_conn_pool_size);
        Add(std::move(dbManager), dbManagerCfgDesc.aliases);
    }
}

void    GpDbManagerCatalog::StopAndClear (void)
{
    iManagers.Clear();
}

void    GpDbManagerCatalog::SStart
(
    const GpDbManagerCfgDesc::C::Vec::SP&   aCfgDescs,
    const GpDbDriverCatalog&                aDbDriverCatalog
)
{
    S().Start(aCfgDescs, aDbDriverCatalog);
}

void    GpDbManagerCatalog::SStopAndClear (void)
{
    S().StopAndClear();
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

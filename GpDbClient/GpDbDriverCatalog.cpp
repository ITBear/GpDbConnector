#include <GpDbConnector/GpDbClient/GpDbDriverCatalog.hpp>
#include <GpDbConnector/GpDbClient/GpDbDriverFactory.hpp>

namespace GPlatform {

GpDbDriverCatalog::GpDbDriverCatalog (void) noexcept
{
}

GpDbDriverCatalog::~GpDbDriverCatalog (void) noexcept
{
}

void    GpDbDriverCatalog::Add (GpSP<GpDbDriverFactory> aFactory)
{
    const GpDbDriverFactory& driverFactory = aFactory.V();

    iCatalog.SetOrUpdate
    (
        std::string(driverFactory.Name()),
        std::move(aFactory)
    );
}

GpDbDriverFactory::SP   GpDbDriverCatalog::Find (std::string_view aName) const
{
    auto res = iCatalog.GetOpt(aName);

    THROW_COND_GP
    (
        res.has_value(),
        [aName]()
        {
            return fmt::format
            (
                "DB driver factory not found by name '{}'",
                aName
            );
        }
    );

    return res.value();
}

}// namespace GPlatform

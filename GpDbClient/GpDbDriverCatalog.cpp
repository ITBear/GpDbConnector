#include <GpDbConnector/GpDbClient/GpDbDriverCatalog.hpp>
#include <GpDbConnector/GpDbClient/GpDbDriverFactory.hpp>

namespace GPlatform {

GpDbDriverCatalog::GpDbDriverCatalog (void) noexcept
{
}

GpDbDriverCatalog::~GpDbDriverCatalog (void) noexcept
{
}

void    GpDbDriverCatalog::Add (GpDbDriverFactory::SP aFactory)
{
    const GpDbDriverFactory& driverFactory = aFactory.V();

    iCatalog.Set
    (
        driverFactory.Name(),
        std::move(aFactory)
    );
}

GpDbDriverFactory::SP   GpDbDriverCatalog::Find (std::string_view aName) const
{
    auto res = iCatalog.FindOpt(aName);

    VERIFY
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

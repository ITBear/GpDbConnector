#include "GpDbDriverCatalog.hpp"
#include "GpDbDriverFactory.hpp"

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
    iCatalog.Register
    (
        std::string(driverFactory.Name()),
        std::move(aFactory)
    );
}

const GpDbDriverFactory&    GpDbDriverCatalog::Find (std::string_view aName) const
{
    auto res = iCatalog.FindOpt(aName);

    THROW_COND_GP
    (
        res.has_value(),
        [&](){return "DB driver factory not found by name '"_sv + aName + "'"_sv;}
    );

    return res.value().get().V();
}

}//namespace GPlatform

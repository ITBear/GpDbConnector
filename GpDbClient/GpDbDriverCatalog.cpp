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

    iCatalog.Set
    (
        std::u8string(driverFactory.Name()),
        std::move(aFactory)
    );
}

const GpDbDriverFactory&    GpDbDriverCatalog::Find (std::u8string_view aName) const
{
    auto res = iCatalog.GetOpt(aName);

    THROW_COND_GP
    (
        res.has_value(),
        [&](){return u8"DB driver factory not found by name '"_sv + aName + u8"'"_sv;}
    );

    return res.value().get().V();
}

}//namespace GPlatform

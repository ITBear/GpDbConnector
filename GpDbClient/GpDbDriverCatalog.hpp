#pragma once

#include "GpDbClient_global.hpp"

namespace GPlatform {

class GpDbDriverFactory;

class GP_DB_CLIENT_API GpDbDriverCatalog
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbDriverCatalog)
    CLASS_DD(GpDbDriverCatalog)
    CLASS_TAG(THREAD_SAFE)

    using CatalogT  = GpElementsCatalog<std::string, GpSP<GpDbDriverFactory>>;

public:
                                GpDbDriverCatalog   (void) noexcept;
                                ~GpDbDriverCatalog  (void) noexcept;

    void                        Add                 (GpSP<GpDbDriverFactory> aFactory);
    const GpDbDriverFactory&    Find                (std::string_view aName) const;

private:
    CatalogT                    iCatalog;
};

}//GPlatform

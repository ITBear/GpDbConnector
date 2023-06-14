#pragma once

#include "GpDbClient_global.hpp"
#include "../../GpCore2/GpUtils/Macro/GpMacroClass.hpp"
#include "../../GpCore2/GpUtils/Types/Containers/GpContainersT.hpp"
#include "../../GpCore2/GpUtils/Types/Containers/GpDictionary.hpp"

namespace GPlatform {

class GpDbDriverFactory;

class GP_DB_CLIENT_API GpDbDriverCatalog
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbDriverCatalog)
    CLASS_DD(GpDbDriverCatalog)
    TAG_SET(THREAD_SAFE)

    using CatalogT  = GpDictionary<std::u8string, GpSP<GpDbDriverFactory>>;

public:
                                GpDbDriverCatalog   (void) noexcept;
                                ~GpDbDriverCatalog  (void) noexcept;

    void                        Add                 (GpSP<GpDbDriverFactory> aFactory);
    const GpDbDriverFactory&    Find                (std::u8string_view aName) const;

private:
    CatalogT                    iCatalog;
};

}//GPlatform

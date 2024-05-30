#pragma once

#include <GpDbConnector/GpDbClient/GpDbClient_global.hpp>

#include <GpCore2/Config/IncludeExt/boost_small_vector.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpDictionary.hpp>

namespace GPlatform {

class GpDbDriverFactory;

class GP_DB_CLIENT_API GpDbDriverCatalog
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbDriverCatalog)
    CLASS_DD(GpDbDriverCatalog)
    TAG_SET(THREAD_SAFE)

    using CatalogT  = GpDictionary
    <
        boost::container::small_flat_map
        <
            std::string,
            GpSP<GpDbDriverFactory>,
            4,
            std::less<>
        >
    >;

public:
                                GpDbDriverCatalog   (void) noexcept;
                                ~GpDbDriverCatalog  (void) noexcept;

    void                        Add                 (GpSP<GpDbDriverFactory> aFactory);
    GpSP<GpDbDriverFactory>     Find                (std::string_view aName) const;

private:
    CatalogT                    iCatalog;
};

}// namespace GPlatform

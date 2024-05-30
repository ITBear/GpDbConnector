#pragma once

#include <GpCore2/Config/IncludeExt/boost_small_vector.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpDbConnector/GpDbClient/GpDbManagerCfgDesc.hpp>
#include <GpDbConnector/GpDbClient/GpDbDriverCatalog.hpp>

namespace GPlatform {

class GpDbManager;

class GP_DB_CLIENT_API GpDbManagerCatalog
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbManagerCatalog)
    CLASS_DD(GpDbManagerCatalog)
    TAG_SET(THREAD_SAFE)

    using ManagersT = GpDictionary<boost::container::small_flat_map<std::string, GpSP<GpDbManager>, 8, std::less<>>>;

private:
                                GpDbManagerCatalog  (void) noexcept;
                                ~GpDbManagerCatalog (void) noexcept;

public:
    static GpDbManagerCatalog&  S                   (void) noexcept {return sInstance;}
    static inline void          SStart              (const GpDbManagerCfgDesc::C::Vec::SP&  aCfgDescs,
                                                     const GpDbDriverCatalog&               aDbDriverCatalog);
    static inline void          SStopAndClear       (void);

    void                        Add                 (GpSP<GpDbManager>                  aManager,
                                                     const std::vector<std::string>&    aAliases);
    GpDbManager&                Find                (std::string_view aName);

private:
    void                        Start               (const GpDbManagerCfgDesc::C::Vec::SP&  aCfgDescs,
                                                     const GpDbDriverCatalog&               aDbDriverCatalog);
    void                        StopAndClear        (void);

private:
    ManagersT                   iManagers;

    static GpDbManagerCatalog   sInstance;
};

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

}// GPlatform

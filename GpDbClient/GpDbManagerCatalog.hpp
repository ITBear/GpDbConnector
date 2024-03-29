#pragma once

#include "GpDbManagerCfgDesc.hpp"
#include "GpDbDriverCatalog.hpp"

namespace GPlatform {

class GpDbManager;

class GP_DB_CLIENT_API GpDbManagerCatalog
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbManagerCatalog)
    CLASS_DD(GpDbManagerCatalog)
    TAG_SET(THREAD_SAFE)

    using ManagersT = GpDictionary<std::u8string, GpSP<GpDbManager>>;

private:
                                GpDbManagerCatalog  (void) noexcept;
                                ~GpDbManagerCatalog (void) noexcept;

public:
    static GpDbManagerCatalog&  S                   (void) noexcept {return sInstance;}
    static void                 SStart              (const GpDbManagerCfgDesc::C::Vec::SP&  aCfgDescs,
                                                     const GpDbDriverCatalog&               aDbDriverCatalog) {S().Start(aCfgDescs, aDbDriverCatalog);}
    static void                 SStopAndClear       (void) {S().StopAndClear();}

    void                        Add                 (GpSP<GpDbManager>                  aManager,
                                                     const std::vector<std::u8string>&  aAliases);
    GpDbManager&                Find                (std::u8string_view aName);

private:
    void                        Start               (const GpDbManagerCfgDesc::C::Vec::SP&  aCfgDescs,
                                                     const GpDbDriverCatalog&               aDbDriverCatalog);
    void                        StopAndClear        (void);

private:
    ManagersT                   iManagers;

    static GpDbManagerCatalog   sInstance;
};

}// GPlatform

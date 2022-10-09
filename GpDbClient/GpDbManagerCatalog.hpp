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
    CLASS_TAG(THREAD_SAFE)

    using ManagersT = GpElementsCatalog<std::string_view, GpSP<GpDbManager>>;

private:
                                GpDbManagerCatalog      (void) noexcept;
                                ~GpDbManagerCatalog     (void) noexcept;

public:
    static GpDbManagerCatalog&  S                       (void) noexcept;

    void                        Clear                   (void);
    void                        Init                    (const GpDbManagerCfgDesc::C::Vec::SP&  aCfgDescs,
                                                         const GpDbDriverCatalog&               aDbDriverCatalog);

    void                        Add                     (GpSP<GpDbManager>                  aManager,
                                                         const std::vector<std::string>&    aAliases);
    GpDbManager&                Find                    (std::string_view aName);

private:
    ManagersT                   iManagers;
};

}//GPlatform

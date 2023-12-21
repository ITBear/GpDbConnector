#include "GpDbManagerCfgDesc.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpDbManagerCfgDesc, GP_MODULE_UUID)

GpDbManagerCfgDesc::~GpDbManagerCfgDesc (void) noexcept
{
}

void    GpDbManagerCfgDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(aliases);
    PROP(driver_name);
    PROP(max_conn_pool_size);
    PROP(connection_str);
    PROP(mode);
    PROP(event_poller_name);
}

}// namespace GPlatform

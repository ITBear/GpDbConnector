#include "GpDbSearchDesc.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpDbSearchDesc, GP_MODULE_UUID)

GpDbSearchDesc::~GpDbSearchDesc (void) noexcept
{
}

void    GpDbSearchDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(filter);
    PROP(limit);
    PROP(min_id);
    PROP(order);
}

}//namespace GPlatform

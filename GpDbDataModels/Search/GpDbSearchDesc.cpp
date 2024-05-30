#include "GpDbSearchDesc.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpDbSearchDesc, GP_MODULE_UUID)

GpDbSearchDesc::~GpDbSearchDesc (void) noexcept
{
}

void    GpDbSearchDesc::_SReflectCollectProps (GpReflectProp::SmallVecVal& aPropsOut)
{
    PROP(filter);
    PROP(limit);
    PROP(min_id);
    PROP(order);
}

}// namespace GPlatform

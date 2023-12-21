#include "GpDbSearchOrderDesc.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpDbSearchOrderDesc, GP_MODULE_UUID)

GpDbSearchOrderDesc::~GpDbSearchOrderDesc (void) noexcept
{
}

void    GpDbSearchOrderDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(name);
    PROP(type);
}

}//namespace GPlatform

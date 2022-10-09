#include "GpDbSearchDesc.hpp"

namespace GPlatform {

REFLECT_IMPLEMENT(GpDbSearchDesc, GP_MODULE_UUID)

GpDbSearchDesc::GpDbSearchDesc (void) noexcept
{
}

GpDbSearchDesc::GpDbSearchDesc (const GpDbSearchDesc& aDesc):
GpReflectObject(aDesc),
filter(aDesc.filter),
limit(aDesc.limit),
order(GpReflectUtils::SCopyValue(aDesc.order))
{
}

GpDbSearchDesc::GpDbSearchDesc (GpDbSearchDesc&& aDesc) noexcept:
GpReflectObject(std::move(aDesc)),
filter(std::move(aDesc.filter)),
limit(std::move(aDesc.limit)),
order(std::move(aDesc.order))
{
}

GpDbSearchDesc::GpDbSearchDesc
(
    std::string                             aFilter,
    const size_t                            aLimit,
    const GpDbSearchOrderDesc::C::Vec::SP&  aOrder
) noexcept:
filter(std::move(aFilter)),
limit(aLimit),
order(aOrder)//TODO: deep copy
{
}

GpDbSearchDesc::~GpDbSearchDesc (void) noexcept
{
}

void    GpDbSearchDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(filter);
    PROP(limit);
    PROP(order);
}

}//namespace GPlatform

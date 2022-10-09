#include "GpDbSearchOrderDesc.hpp"

namespace GPlatform {

REFLECT_IMPLEMENT(GpDbSearchOrderDesc, GP_MODULE_UUID)

GpDbSearchOrderDesc::GpDbSearchOrderDesc (void) noexcept
{
}

GpDbSearchOrderDesc::GpDbSearchOrderDesc (const GpDbSearchOrderDesc& aDesc):
GpReflectObject(aDesc),
name(aDesc.name),
type(aDesc.type)
{
}

GpDbSearchOrderDesc::GpDbSearchOrderDesc (GpDbSearchOrderDesc&& aDesc) noexcept:
GpReflectObject(std::move(aDesc)),
name(std::move(aDesc.name)),
type(std::move(aDesc.type))
{
}

GpDbSearchOrderDesc::GpDbSearchOrderDesc
(
    std::string_view                    aName,
    const GpDbSearchOrderType::EnumT    aType
):
name(aName),
type(aType)
{
}

GpDbSearchOrderDesc::GpDbSearchOrderDesc
(
    std::string                         aName,
    const GpDbSearchOrderType::EnumT    aType
) noexcept:
name(std::move(aName)),
type(aType)
{
}

GpDbSearchOrderDesc::~GpDbSearchOrderDesc (void) noexcept
{
}

void    GpDbSearchOrderDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(name);
    PROP(type);
}

}//namespace GPlatform

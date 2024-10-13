#include <GpDbConnector/GpDbDataModels/Search/GpDbSearchOrderDesc.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpDbSearchOrderDesc, GP_MODULE_UUID)

GpDbSearchOrderDesc::GpDbSearchOrderDesc (const GpDbSearchOrderDesc& aDesc):
GpReflectObject{aDesc},
name{GpReflectUtils::SCopyValue(aDesc.name)},
type{GpReflectUtils::SCopyValue(aDesc.type)}
{
}

GpDbSearchOrderDesc::GpDbSearchOrderDesc (GpDbSearchOrderDesc&& aDesc) noexcept:
GpReflectObject{std::move(aDesc)},
name{GpReflectUtils::SCopyValue(std::move(aDesc.name))},
type{GpReflectUtils::SCopyValue(std::move(aDesc.type))}
{
}

GpDbSearchOrderDesc::GpDbSearchOrderDesc
(
    std::string                         aName,
    const GpDbSearchOrderType::EnumT    aType
) noexcept:
name{std::move(aName)},
type{aType}
{
}

GpDbSearchOrderDesc::~GpDbSearchOrderDesc (void) noexcept
{
}

void    GpDbSearchOrderDesc::_SReflectCollectProps (GpReflectProp::SmallVecVal& aPropsOut)
{
    PROP(name);
    PROP(type);
}

}// namespace GPlatform

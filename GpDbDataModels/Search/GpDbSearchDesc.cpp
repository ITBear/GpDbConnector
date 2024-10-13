#include <GpDbConnector/GpDbDataModels/Search/GpDbSearchDesc.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpDbSearchDesc, GP_MODULE_UUID)

GpDbSearchDesc::GpDbSearchDesc (const GpDbSearchDesc& aDesc):
GpReflectObject{aDesc},
filter{GpReflectUtils::SCopyValue(aDesc.filter)},
limit {GpReflectUtils::SCopyValue(aDesc.limit)},
min_id{GpReflectUtils::SCopyValue(aDesc.min_id)},
order {GpReflectUtils::SCopyValue(aDesc.order)}
{
}

GpDbSearchDesc::GpDbSearchDesc (GpDbSearchDesc&& aDesc) noexcept:
GpReflectObject{std::move(aDesc)},
filter{std::move(aDesc.filter)},
limit {std::move(aDesc.limit)},
min_id{std::move(aDesc.min_id)},
order {std::move(aDesc.order)}
{
}

GpDbSearchDesc::GpDbSearchDesc
(
    std::string                     aFilter,
    const u_int_32                  aLimit,
    const u_int_32                  aMinId,
    GpDbSearchOrderDesc::C::Vec::SP aOrder
) noexcept:
filter{std::move(aFilter)},
limit {std::move(aLimit)},
min_id{std::move(aMinId)},
order {std::move(aOrder)}
{
}

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

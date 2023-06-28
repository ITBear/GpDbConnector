#pragma once

#include "GpDbSearchOrderDesc.hpp"

namespace GPlatform {

class GP_DB_DATA_MODELS_API GpDbSearchDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpDbSearchDesc)
    REFLECT_DECLARE(u8"6cd0aa57-3ea6-4c1c-8886-8519164566b1"_uuid)

public:
                                    GpDbSearchDesc  (void) noexcept = default;
    inline                          GpDbSearchDesc  (const GpDbSearchDesc& aDesc);
    inline                          GpDbSearchDesc  (GpDbSearchDesc&& aDesc) noexcept;
    inline                          GpDbSearchDesc  (std::u8string                      aFilter,
                                                     const u_int_32                     aLimit,
                                                     GpDbSearchOrderDesc::C::Vec::SP    aOrder);
    virtual                         ~GpDbSearchDesc (void) noexcept override final;

public:
    std::u8string                   filter;
    u_int_32                        limit = 0;
    GpDbSearchOrderDesc::C::Vec::SP order;
};

GpDbSearchDesc::GpDbSearchDesc (const GpDbSearchDesc& aDesc):
GpReflectObject(aDesc),
filter(GpReflectUtils::SCopyValue(aDesc.filter)),
limit (GpReflectUtils::SCopyValue(aDesc.limit)),
order (GpReflectUtils::SCopyValue(aDesc.order))
{
}

GpDbSearchDesc::GpDbSearchDesc (GpDbSearchDesc&& aDesc) noexcept:
GpReflectObject(std::move(aDesc)),
filter(std::move(aDesc.filter)),
limit (std::move(aDesc.limit)),
order (std::move(aDesc.order))
{
}

GpDbSearchDesc::GpDbSearchDesc
(
    std::u8string                   aFilter,
    const u_int_32                  aLimit,
    GpDbSearchOrderDesc::C::Vec::SP aOrder
):
filter(std::move(aFilter)),
limit (aLimit),
order (std::move(aOrder))
{
}

}//namespace GPlatform

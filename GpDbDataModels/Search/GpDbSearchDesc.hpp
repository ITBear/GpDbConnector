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
                                                     const u_int_32                     aMinId,
                                                     GpDbSearchOrderDesc::C::Vec::SP    aOrder) noexcept;
    virtual                         ~GpDbSearchDesc (void) noexcept override final;

public:
    std::u8string                   filter;
    u_int_32                        limit   = 0;
    u_int_32                        min_id  = 0;
    GpDbSearchOrderDesc::C::Vec::SP order;
};

GpDbSearchDesc::GpDbSearchDesc (const GpDbSearchDesc& aDesc):
GpReflectObject(aDesc),
filter(GpReflectUtils::SCopyValue(aDesc.filter)),
limit (GpReflectUtils::SCopyValue(aDesc.limit)),
min_id(GpReflectUtils::SCopyValue(aDesc.min_id)),
order (GpReflectUtils::SCopyValue(aDesc.order))
{
}

GpDbSearchDesc::GpDbSearchDesc (GpDbSearchDesc&& aDesc) noexcept:
GpReflectObject(std::move(aDesc)),
filter(std::move(aDesc.filter)),
limit (std::move(aDesc.limit)),
min_id(std::move(aDesc.min_id)),
order (std::move(aDesc.order))
{
}

GpDbSearchDesc::GpDbSearchDesc
(
    std::u8string                   aFilter,
    const u_int_32                  aLimit,
    const u_int_32                  aMinId,
    GpDbSearchOrderDesc::C::Vec::SP aOrder
) noexcept:
filter(std::move(aFilter)),
limit (std::move(aLimit)),
min_id(std::move(aMinId)),
order (std::move(aOrder))
{
}

}//namespace GPlatform

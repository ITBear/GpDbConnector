#pragma once

#include "GpDbSearchOrderType.hpp"

#include <GpCore2/GpReflection/GpReflectObject.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>

namespace GPlatform {

class GP_DB_DATA_MODELS_API GpDbSearchOrderDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpDbSearchOrderDesc)
    REFLECT_DECLARE("64e04616-1c01-48d4-a00d-54816305a00e"_uuid)

public:
                            GpDbSearchOrderDesc     (void) noexcept = default;
    inline                  GpDbSearchOrderDesc     (const GpDbSearchOrderDesc& aDesc);
    inline                  GpDbSearchOrderDesc     (GpDbSearchOrderDesc&& aDesc) noexcept;
    inline                  GpDbSearchOrderDesc     (std::string                        aName,
                                                     const GpDbSearchOrderType::EnumT   aType) noexcept;
    virtual                 ~GpDbSearchOrderDesc    (void) noexcept override final;

public:
    std::string             name;
    GpDbSearchOrderType     type;
};

GpDbSearchOrderDesc::GpDbSearchOrderDesc (const GpDbSearchOrderDesc& aDesc):
GpReflectObject(aDesc),
name(GpReflectUtils::SCopyValue(aDesc.name)),
type(GpReflectUtils::SCopyValue(aDesc.type))
{
}

GpDbSearchOrderDesc::GpDbSearchOrderDesc (GpDbSearchOrderDesc&& aDesc) noexcept:
GpReflectObject(std::move(aDesc)),
name(GpReflectUtils::SCopyValue(std::move(aDesc.name))),
type(GpReflectUtils::SCopyValue(std::move(aDesc.type)))
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

}// namespace GPlatform

#pragma once

#include "GpDbSearchOrderType.hpp"

namespace GPlatform {

class GP_DB_DATA_MODELS_API GpDbSearchOrderDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpDbSearchOrderDesc)
    REFLECT_DECLARE("64e04616-1c01-48d4-a00d-54816305a00e"_uuid)

public:
                                GpDbSearchOrderDesc     (void) noexcept;
                                GpDbSearchOrderDesc     (const GpDbSearchOrderDesc& aDesc);
                                GpDbSearchOrderDesc     (GpDbSearchOrderDesc&& aDesc) noexcept;
                                GpDbSearchOrderDesc     (std::string_view                   aName,
                                                         const GpDbSearchOrderType::EnumT   aType);
                                GpDbSearchOrderDesc     (std::string                        aName,
                                                         const GpDbSearchOrderType::EnumT   aType) noexcept;
    virtual                     ~GpDbSearchOrderDesc    (void) noexcept override final;

public:
    std::string                 name;
    GpDbSearchOrderType         type;
};

}//namespace GPlatform

#pragma once

#include <GpDbConnector/GpDbDataModels/Search/GpDbSearchOrderDesc.hpp>

namespace GPlatform {

class GP_DB_DATA_MODELS_API GpDbSearchDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpDbSearchDesc)
    REFLECT_DECLARE("6cd0aa57-3ea6-4c1c-8886-8519164566b1"_uuid)

public:
                                    GpDbSearchDesc  (void) noexcept = default;
                                    GpDbSearchDesc  (const GpDbSearchDesc& aDesc);
                                    GpDbSearchDesc  (GpDbSearchDesc&& aDesc) noexcept;
                                    GpDbSearchDesc  (std::string                        aFilter,
                                                     u_int_32                           aLimit,
                                                     u_int_32                           aMinId,
                                                     GpDbSearchOrderDesc::C::Vec::SP    aOrder) noexcept;
    virtual                         ~GpDbSearchDesc (void) noexcept override final;

public:
    std::string                     filter;
    u_int_32                        limit   = 0;
    u_int_32                        min_id  = 0;
    GpDbSearchOrderDesc::C::Vec::SP order;
};

}// namespace GPlatform

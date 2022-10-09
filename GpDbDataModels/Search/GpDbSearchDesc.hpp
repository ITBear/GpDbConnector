#pragma once

#include "GpDbSearchOrderDesc.hpp"

namespace GPlatform {

class GP_DB_DATA_MODELS_API GpDbSearchDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpDbSearchDesc)
    REFLECT_DECLARE("6cd0aa57-3ea6-4c1c-8886-8519164566b1"_uuid)

public:
                                    GpDbSearchDesc  (void) noexcept;
                                    GpDbSearchDesc  (const GpDbSearchDesc& aDesc);
                                    GpDbSearchDesc  (GpDbSearchDesc&& aDesc) noexcept;
                                    GpDbSearchDesc  (std::string                            aFilter,
                                                     const size_t                           aLimit,
                                                     const GpDbSearchOrderDesc::C::Vec::SP& aOrder) noexcept;
    virtual                         ~GpDbSearchDesc (void) noexcept override final;

public:
    std::string                     filter;
    size_t                          limit = 0;
    GpDbSearchOrderDesc::C::Vec::SP order;
};

}//namespace GPlatform

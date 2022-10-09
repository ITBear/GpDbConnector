#pragma once

#include "GpDbConnectionMode.hpp"

namespace GPlatform {

class GpDbDriver;

class GpDbDriverFactory
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbDriverFactory)
    CLASS_DD(GpDbDriverFactory)

protected:
    inline                          GpDbDriverFactory   (std::string aName) noexcept;

public:
    virtual                         ~GpDbDriverFactory  (void) noexcept = default;

    std::string_view                Name                (void) const noexcept {return  iName;}

    virtual GpSP<GpDbDriver>        NewInstance         (const GpDbConnectionMode::EnumT    aMode,
                                                         GpIOEventPoller::SP                aEventPoller) const = 0;

private:
    const std::string               iName;
};

GpDbDriverFactory::GpDbDriverFactory (std::string aName) noexcept:
iName(std::move(aName))
{
}

}//namespace GPlatform

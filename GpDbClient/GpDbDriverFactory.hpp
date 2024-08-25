#pragma once

#include <GpDbConnector/GpDbClient/GpDbDriver.hpp>

namespace GPlatform {

class GP_DB_CLIENT_API GpDbDriverFactory
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbDriverFactory)
    CLASS_DD(GpDbDriverFactory)

protected:
                            GpDbDriverFactory   (std::string aName) noexcept;

public:
    virtual                 ~GpDbDriverFactory  (void) noexcept = default;

    std::string_view        Name                (void) const noexcept {return iName;}

    virtual GpDbDriver::SP  NewInstance         (void) const = 0;

private:
    const std::string       iName;
};

}// namespace GPlatform

#include <GpDbConnector/GpDbClient/GpDbDriver.hpp>

namespace GPlatform {

GpDbDriver::GpDbDriver (std::string aName) noexcept:
iName{std::move(aName)}
{
}

GpDbDriver::~GpDbDriver (void) noexcept
{
}

}// namespace GPlatform

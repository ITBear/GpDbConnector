#include <GpDbConnector/GpDbClient/GpDbDriverFactory.hpp>

namespace GPlatform {

GpDbDriverFactory::GpDbDriverFactory (std::string aName) noexcept:
iName{std::move(aName)}
{
}

}// namespace GPlatform

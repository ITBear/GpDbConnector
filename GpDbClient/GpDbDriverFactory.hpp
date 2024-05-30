#pragma once

#include <GpDbConnector/GpDbClient/GpDbConnectionMode.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPoller.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>

namespace GPlatform {

class GpDbDriver;

class GpDbDriverFactory
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbDriverFactory)
    CLASS_DD(GpDbDriverFactory)

protected:
    inline                      GpDbDriverFactory   (std::string aName) noexcept;

public:
    virtual                     ~GpDbDriverFactory  (void) noexcept = default;

    std::string_view            Name                (void) const noexcept {return  iName;}

    virtual GpSP<GpDbDriver>    NewInstance         (GpDbConnectionMode::EnumT  aMode,
                                                     GpIOEventPollerIdx         aIOEventPollerIdx) const = 0;

private:
    const std::string           iName;
};

GpDbDriverFactory::GpDbDriverFactory (std::string aName) noexcept:
iName{std::move(aName)}
{
}

}// namespace GPlatform

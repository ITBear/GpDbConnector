#pragma once

#include "GpDbConnectionMode.hpp"
#include "Query/GpDbQueryPrepared.hpp"
#include "../../GpNetwork/GpNetworkCore/Pollers/GpIOEventPoller.hpp"

namespace GPlatform {

class GpDbConnection;

class GpDbDriver
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbDriver)
    CLASS_DD(GpDbDriver)
    TAG_SET(THREAD_SAFE)

protected:
    inline                          GpDbDriver      (std::u8string                      aName,
                                                     const GpDbConnectionMode::EnumT    aMode,
                                                     GpIOEventPoller::SP                aEventPoller) noexcept;

public:
    virtual                         ~GpDbDriver     (void) noexcept = default;

    std::u8string_view              Name            (void) const noexcept {return iName;}
    GpDbConnectionMode::EnumT       Mode            (void) const noexcept {return iMode;}
    GpIOEventPoller::SP             EventPoller     (void) const noexcept {return iEventPoller;}

    virtual GpSP<GpDbConnection>    NewConnection   (std::u8string_view aConnStr) const = 0;
    virtual GpDbQueryPrepared::CSP  Prepare         (const GpDbQuery& aQuery) const = 0;

private:
    const std::u8string             iName;
    const GpDbConnectionMode::EnumT iMode;
    GpIOEventPoller::SP             iEventPoller;
};

GpDbDriver::GpDbDriver
(
    std::u8string                   aName,
    const GpDbConnectionMode::EnumT aMode,
    GpIOEventPoller::SP             aEventPoller
) noexcept:
iName       (std::move(aName)),
iMode       (aMode),
iEventPoller(std::move(aEventPoller))
{
}

}//GPlatform

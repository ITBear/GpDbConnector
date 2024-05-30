#pragma once

#include <GpDbConnector/GpDbClient/GpDbConnectionMode.hpp>
#include <GpDbConnector/GpDbClient/Query/GpDbQueryPrepared.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPoller.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>

namespace GPlatform {

class GpDbConnection;

class GpDbDriver
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbDriver)
    CLASS_DD(GpDbDriver)
    TAG_SET(THREAD_SAFE)

protected:
    inline                          GpDbDriver          (std::string                aName,
                                                         GpDbConnectionMode::EnumT  aMode,
                                                         GpIOEventPollerIdx         aIOEventPollerIdx) noexcept;

public:
    virtual                         ~GpDbDriver         (void) noexcept = default;

    std::string_view                Name                (void) const noexcept {return iName;}
    GpDbConnectionMode::EnumT       Mode                (void) const noexcept {return iMode;}
    GpIOEventPollerIdx              IOEventPollerIdx    (void) const noexcept {return iIOEventPollerIdx;}

    virtual GpSP<GpDbConnection>    NewConnection       (std::string_view aConnStr) const = 0;
    virtual GpDbQueryPrepared::CSP  Prepare             (const GpDbQuery& aQuery) const = 0;

private:
    const std::string               iName;
    const GpDbConnectionMode::EnumT iMode;
    const GpIOEventPollerIdx        iIOEventPollerIdx;
};

GpDbDriver::GpDbDriver
(
    std::string                     aName,
    const GpDbConnectionMode::EnumT aMode,
    const GpIOEventPollerIdx        aIOEventPollerIdx
) noexcept:
iName            {std::move(aName)},
iMode            {aMode},
iIOEventPollerIdx{aIOEventPollerIdx}
{
}

}// namespace GPlatform

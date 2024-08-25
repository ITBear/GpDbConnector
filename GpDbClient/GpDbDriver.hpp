#pragma once

#include <GpDbConnector/GpDbClient/GpDbConnection.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>

namespace GPlatform {

class GP_DB_CLIENT_API GpDbDriver
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbDriver)
    CLASS_DD(GpDbDriver)
    TAG_SET(THREAD_SAFE)

protected:
                                GpDbDriver      (std::string aName) noexcept;

public:
    virtual                     ~GpDbDriver     (void) noexcept;

    std::string_view            Name            (void) const noexcept {return iName;}

    virtual GpDbConnection::SP  NewConnection   (GpIOEventPollerIdx aIOEventPollerIdx,
                                                 milliseconds_t     aConnectTimeout,
                                                 std::string        aServerHost,
                                                 u_int_16           aServerPort,
                                                 std::string        aUserName,
                                                 std::string        aPassword,
                                                 std::string        aDatabase) const = 0;

private:
    const std::string           iName;
};

}// namespace GPlatform

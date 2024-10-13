#pragma once

#include <GpDbConnector/GpDbClient/GpDbDriver.hpp>

namespace GPlatform {

class GpDbDriverPgSql final: public GpDbDriver
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbDriverPgSql)
    CLASS_DD(GpDbDriverPgSql)
    TAG_SET(THREAD_SAFE)

public:
                                GpDbDriverPgSql     (void);
    virtual                     ~GpDbDriverPgSql    (void) noexcept override final;

    virtual GpDbConnection::SP  NewConnection       (GpIOEventPollerIdx aIOEventPollerIdx,
                                                     milliseconds_t     aConnectTimeout,
                                                     std::string        aServerHost,
                                                     u_int_16           aServerPort,
                                                     std::string        aUserName,
                                                     std::string        aPassword,
                                                     std::string        aDatabase) const override final;
};

}// namespace GPlatform

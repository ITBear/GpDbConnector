#pragma once

#include "GpDbPostgreSql_global.hpp"
#include <postgresql/libpq-fe.h>

namespace GPlatform {

class GpDbDriverPgSql final: public GpDbDriver
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbDriverPgSql)
    CLASS_DD(GpDbDriverPgSql)
    CLASS_TAG(THREAD_SAFE)

public:
    inline                          GpDbDriverPgSql     (const GpDbConnectionMode::EnumT    aMode,
                                                         GpIOEventPoller::SP                aEventPoller);
    virtual                         ~GpDbDriverPgSql    (void) noexcept override final;

    virtual GpDbConnection::SP      NewConnection       (std::string_view aConnStr) const override final;
    virtual GpDbQueryPrepared::CSP  Prepare             (const GpDbQuery& aQuery) const override final;

private:
    PGconn*                         ConnectSync         (std::string_view aConnStr) const;
    PGconn*                         ConnectAsync        (std::string_view aConnStr) const;
};

GpDbDriverPgSql::GpDbDriverPgSql
(
    const GpDbConnectionMode::EnumT aMode,
    GpIOEventPoller::SP             aEventPoller
):
GpDbDriver
(
    "postgresql",
     aMode,
     std::move(aEventPoller)
)
{
}

}//namespace GPlatform

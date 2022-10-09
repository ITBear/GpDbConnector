#pragma once

#include "GpDbPostgreSql_global.hpp"

namespace GPlatform {

class GP_DB_POSTGRESQL_API GpDbDriverFactoryPgSql final: public GpDbDriverFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbDriverFactoryPgSql)
    CLASS_DD(GpDbDriverFactoryPgSql)

public:
                                    GpDbDriverFactoryPgSql  (void);
    virtual                         ~GpDbDriverFactoryPgSql (void) noexcept override final;

    virtual GpDbDriver::SP          NewInstance             (const GpDbConnectionMode::EnumT    aMode,
                                                             GpIOEventPoller::SP                aEventPoller) const override final;
};

}//namespace GPlatform

#pragma once

#include <GpDbConnector/GpDbPostgreSql/GpDbPostgreSql_global.hpp>
#include <GpDbConnector/GpDbClient/GpDbDriverFactory.hpp>
#include <GpDbConnector/GpDbClient/GpDbDriver.hpp>

namespace GPlatform {

class GP_DB_POSTGRESQL_API GpDbDriverFactoryPgSql final: public GpDbDriverFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbDriverFactoryPgSql)
    CLASS_DD(GpDbDriverFactoryPgSql)

public:
                            GpDbDriverFactoryPgSql  (void);
    virtual                 ~GpDbDriverFactoryPgSql (void) noexcept override final;

    virtual GpDbDriver::SP  NewInstance             (void) const override final;
};

}// namespace GPlatform

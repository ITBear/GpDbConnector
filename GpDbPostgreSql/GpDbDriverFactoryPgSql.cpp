#include <GpDbConnector/GpDbPostgreSql/GpDbDriverFactoryPgSql.hpp>
#include <GpDbConnector/GpDbPostgreSql/GpDbDriverPgSql.hpp>

namespace GPlatform {

GpDbDriverFactoryPgSql::GpDbDriverFactoryPgSql (void):
GpDbDriverFactory{"postgresql"}
{
}

GpDbDriverFactoryPgSql::~GpDbDriverFactoryPgSql (void) noexcept
{
}

GpDbDriver::SP  GpDbDriverFactoryPgSql::NewInstance (void) const
{
    return MakeSP<GpDbDriverPgSql>();
}

}// namespace GPlatform

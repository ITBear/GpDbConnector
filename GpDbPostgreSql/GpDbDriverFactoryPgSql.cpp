#include "GpDbDriverFactoryPgSql.hpp"
#include "GpDbDriverPgSql.hpp"

namespace GPlatform {

GpDbDriverFactoryPgSql::GpDbDriverFactoryPgSql (void):
GpDbDriverFactory{"postgresql"}
{
}

GpDbDriverFactoryPgSql::~GpDbDriverFactoryPgSql (void) noexcept
{
}

GpDbDriver::SP  GpDbDriverFactoryPgSql::NewInstance
(
    const GpDbConnectionMode::EnumT aMode,
    const GpIOEventPollerIdx        aIOEventPollerIdx
) const
{
    return MakeSP<GpDbDriverPgSql>
    (
        aMode,
        aIOEventPollerIdx
    );
}

}// namespace GPlatform

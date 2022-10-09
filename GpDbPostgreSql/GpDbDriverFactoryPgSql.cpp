#include "GpDbDriverFactoryPgSql.hpp"
#include "GpDbDriverPgSql.hpp"

namespace GPlatform {

GpDbDriverFactoryPgSql::GpDbDriverFactoryPgSql (void):
GpDbDriverFactory("postgresql")
{
}

GpDbDriverFactoryPgSql::~GpDbDriverFactoryPgSql (void) noexcept
{
}

GpDbDriver::SP  GpDbDriverFactoryPgSql::NewInstance
(
    const GpDbConnectionMode::EnumT aMode,
    GpIOEventPoller::SP             aEventPoller
) const
{
    return MakeSP<GpDbDriverPgSql>
    (
        aMode,
        std::move(aEventPoller)
    );
}

}//namespace GPlatform

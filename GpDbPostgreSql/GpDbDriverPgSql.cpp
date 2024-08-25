#include <GpDbConnector/GpDbPostgreSql/GpDbDriverPgSql.hpp>
#include <GpDbConnector/GpDbPostgreSql/GpDbConnectionPgSql.hpp>

namespace GPlatform {

GpDbDriverPgSql::GpDbDriverPgSql (void):
GpDbDriver{"postgresql"}
{
}

GpDbDriverPgSql::~GpDbDriverPgSql (void) noexcept
{
}

GpDbConnection::SP  GpDbDriverPgSql::NewConnection
(
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    const milliseconds_t        aConnectTimeout,
    std::string                 aServerHost,
    const u_int_16              aServerPort,
    std::string                 aUserName,
    std::string                 aPassword,
    std::string                 aDatabase
) const
{
    GpDbConnection::SP connection = MakeSP<GpDbConnectionPgSql>
    (
        aIOEventPollerIdx,
        aConnectTimeout,
        std::move(aServerHost),
        aServerPort,
        std::move(aUserName),
        std::move(aPassword),
        std::move(aDatabase)
    );

    return connection;
}

}// namespace GPlatform

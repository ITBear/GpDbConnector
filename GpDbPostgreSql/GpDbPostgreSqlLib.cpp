#include <GpDbConnector/GpDbPostgreSql/GpDbPostgreSqlLib.hpp>
#include <GpCore2/GpUtils/Other/GpLinkedLibsInfo.hpp>

GP_STATIC_INITIALIZER_IMPL(GpDbPostgreSql)
GP_LIB_REGISTRATOR(GpDbPostgreSqlLib)

void    GpDbPostgreSql_StaticInitializer::OnInitialize (void)
{
    GpDbPostgreSqlLib::SRegisterSelf();
}

#include <GpDbConnector/GpDbClient/GpDbClientLib.hpp>
#include <GpCore2/GpUtils/Other/GpLinkedLibsInfo.hpp>

GP_STATIC_INITIALIZER_IMPL(GpDbClient)
GP_LIB_REGISTRATOR(GpDbClientLib)

void    GpDbClient_StaticInitializer::OnInitialize (void)
{
    GpDbClientLib::SRegisterSelf();
}

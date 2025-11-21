#include <GpDbConnector/GpDbDataModels/GpDbDataModelsLib.hpp>
#include <GpCore2/GpUtils/Other/GpLinkedLibsInfo.hpp>

GP_STATIC_INITIALIZER_IMPL(GpDbDataModels)
GP_LIB_REGISTRATOR(GpDbDataModelsLib)

void    GpDbDataModels_StaticInitializer::OnInitialize (void)
{
    GpDbDataModelsLib::SRegisterSelf();
}

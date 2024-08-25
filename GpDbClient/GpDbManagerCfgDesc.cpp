#include <GpDbConnector/GpDbClient/GpDbManagerCfgDesc.hpp>

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpDbManagerCfgDesc, GP_MODULE_UUID)

GpDbManagerCfgDesc::GpDbManagerCfgDesc (const GpDbManagerCfgDesc& aDesc):
GpReflectObject{aDesc},
aliases           {GpReflectUtils::SCopyValue(aDesc.aliases)},
driver_name       {GpReflectUtils::SCopyValue(aDesc.driver_name)},
max_conn_pool_size{GpReflectUtils::SCopyValue(aDesc.max_conn_pool_size)},
event_poller_name {GpReflectUtils::SCopyValue(aDesc.event_poller_name)},
connect_timeout   {GpReflectUtils::SCopyValue(aDesc.connect_timeout)},
db_host           {GpReflectUtils::SCopyValue(aDesc.db_host)},
db_port           {GpReflectUtils::SCopyValue(aDesc.db_port)},
db_name           {GpReflectUtils::SCopyValue(aDesc.db_name)},
db_user_name      {GpReflectUtils::SCopyValue(aDesc.db_user_name)},
db_password       {GpReflectUtils::SCopyValue(aDesc.db_password)}
{
}

GpDbManagerCfgDesc::GpDbManagerCfgDesc (GpDbManagerCfgDesc&& aDesc) noexcept:
GpReflectObject{std::move(aDesc)},
aliases           {std::move(aDesc.aliases)},
driver_name       {std::move(aDesc.driver_name)},
max_conn_pool_size{std::move(aDesc.max_conn_pool_size)},
event_poller_name {std::move(aDesc.event_poller_name)},
connect_timeout   {std::move(aDesc.connect_timeout)},
db_host           {std::move(aDesc.db_host)},
db_port           {std::move(aDesc.db_port)},
db_name           {std::move(aDesc.db_name)},
db_user_name      {std::move(aDesc.db_user_name)},
db_password       {std::move(aDesc.db_password)}
{
}

GpDbManagerCfgDesc::~GpDbManagerCfgDesc (void) noexcept
{
}

void    GpDbManagerCfgDesc::_SReflectCollectProps (GpReflectProp::SmallVecVal& aPropsOut)
{
    PROP(aliases);
    PROP(driver_name);
    PROP(max_conn_pool_size);
    PROP(event_poller_name);
    PROP(connect_timeout);
    PROP(db_host);
    PROP(db_port);
    PROP(db_name);
    PROP(db_user_name);
    PROP(db_password);
}

}// namespace GPlatform

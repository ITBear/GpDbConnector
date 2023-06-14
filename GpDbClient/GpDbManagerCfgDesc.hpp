#pragma once

#include "GpDbConnectionMode.hpp"
#include "../../GpCore2/GpReflection/GpReflectObject.hpp"
#include "../../GpCore2/GpReflection/GpReflectUtils.hpp"

namespace GPlatform {

class GP_DB_CLIENT_API GpDbManagerCfgDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpDbManagerCfgDesc)
    REFLECT_DECLARE(u8"e200f131-8953-43f6-9ecb-c321b7b00e97"_uuid)

public:
                                    GpDbManagerCfgDesc  (void) noexcept = default;
    inline                          GpDbManagerCfgDesc  (const GpDbManagerCfgDesc& aDesc);
    inline                          GpDbManagerCfgDesc  (GpDbManagerCfgDesc&& aDesc) noexcept;
    virtual                         ~GpDbManagerCfgDesc (void) noexcept override final;

public:
    std::vector<std::u8string>      aliases;
    std::u8string                   driver_name;
    size_t                          max_conn_pool_size;
    std::u8string                   connection_str;
    GpDbConnectionMode              mode;
    std::u8string                   event_poller;
};

GpDbManagerCfgDesc::GpDbManagerCfgDesc (const GpDbManagerCfgDesc& aDesc):
GpReflectObject(aDesc),
aliases           (GpReflectUtils::SCopyValue(aDesc.aliases)),
driver_name       (GpReflectUtils::SCopyValue(aDesc.driver_name)),
max_conn_pool_size(GpReflectUtils::SCopyValue(aDesc.max_conn_pool_size)),
connection_str    (GpReflectUtils::SCopyValue(aDesc.connection_str)),
mode              (GpReflectUtils::SCopyValue(aDesc.mode)),
event_poller      (GpReflectUtils::SCopyValue(aDesc.event_poller))
{
}

GpDbManagerCfgDesc::GpDbManagerCfgDesc (GpDbManagerCfgDesc&& aDesc) noexcept:
GpReflectObject(std::move(aDesc)),
aliases           (std::move(aDesc.aliases)),
driver_name       (std::move(aDesc.driver_name)),
max_conn_pool_size(std::move(aDesc.max_conn_pool_size)),
connection_str    (std::move(aDesc.connection_str)),
mode              (std::move(aDesc.mode)),
event_poller      (std::move(aDesc.event_poller))
{
}

}//namespace GPlatform

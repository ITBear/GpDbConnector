#pragma once

#include <GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>
#include <GpDbConnector/GpDbClient/GpDbClient_global.hpp>

namespace GPlatform {

class GP_DB_CLIENT_API GpDbManagerCfgDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpDbManagerCfgDesc)
    REFLECT_DECLARE("e200f131-8953-43f6-9ecb-c321b7b00e97"_uuid)

public:
                                GpDbManagerCfgDesc  (void) noexcept = default;
                                GpDbManagerCfgDesc  (const GpDbManagerCfgDesc& aDesc);
                                GpDbManagerCfgDesc  (GpDbManagerCfgDesc&& aDesc) noexcept;
    virtual                     ~GpDbManagerCfgDesc (void) noexcept override final;

public:
    std::vector<std::string>    aliases;
    std::string                 driver_name;
    size_t                      max_conn_pool_size  = 16;
    std::string                 event_poller_name;
    milliseconds_t              connect_timeout     = 1.0_si_s;
    std::string                 db_host;
    u_int_16                    db_port             = 0;
    std::string                 db_name;
    std::string                 db_user_name;
    std::string                 db_password;
};

}// namespace GPlatform

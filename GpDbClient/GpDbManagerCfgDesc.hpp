#pragma once

#include "GpDbConnectionMode.hpp"

namespace GPlatform {

class GP_DB_CLIENT_API GpDbManagerCfgDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpDbManagerCfgDesc)
    REFLECT_DECLARE("e200f131-8953-43f6-9ecb-c321b7b00e97"_uuid)

public:
                                    GpDbManagerCfgDesc  (void) noexcept;
                                    GpDbManagerCfgDesc  (const GpDbManagerCfgDesc& aDesc);
                                    GpDbManagerCfgDesc  (GpDbManagerCfgDesc&& aDesc) noexcept;
    virtual                         ~GpDbManagerCfgDesc (void) noexcept override final;

    const std::vector<std::string>& Aliases             (void) const noexcept {return aliases;}
    std::string_view                DriverName          (void) const noexcept {return driver_name;}
    size_t                          MaxConnPoolSize     (void) const noexcept {return max_conn_pool_size;}
    std::string_view                ConnectionStr       (void) const noexcept {return connection_str;}
    GpDbConnectionMode::EnumT       Mode                (void) const noexcept {return mode.Value();}
    std::string_view                EventPoller         (void) const noexcept {return event_poller;}

private:
    std::vector<std::string>        aliases;
    std::string                     driver_name;
    size_t                          max_conn_pool_size;
    std::string                     connection_str;
    GpDbConnectionMode              mode;
    std::string                     event_poller;
};

}//namespace GPlatform

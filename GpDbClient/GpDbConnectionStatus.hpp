#pragma once

#include "GpDbClient_global.hpp"

namespace GPlatform {

GP_ENUM(GP_DB_CLIENT_API, GpDbConnectionStatus,
    CLOSED,
    CONNECTION_IN_PROGRESS,
    CONNECTED
);

}//GPlatform

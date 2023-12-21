#pragma once

#include "GpDbClient_global.hpp"

#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

GP_ENUM(GP_DB_CLIENT_API, GpDbConnectionMode,
    SYNC,
    ASYNC
);

}//namespace GPlatform

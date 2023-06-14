#pragma once

#include "../GpDbClient_global.hpp"
#include "../../../GpCore2/GpUtils/Types/Enums/GpEnum.hpp"
#include "../../../GpCore2/GpUtils/Types/Strings/GpStringOps.hpp"

namespace GPlatform {

GP_ENUM(GP_DB_CLIENT_API, GpDbQueryResState,
    NOT_CREATED,
    IN_PROGRESS,
    COMPLETE_NO_RETURN_DATA,
    COMPLETE_HAS_RETURN_DATA,
    ERROR_OCCURRED
);

}//namespace GPlatform

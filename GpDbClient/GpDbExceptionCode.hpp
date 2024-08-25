#pragma once

#include <GpDbConnector/GpDbClient/GpDbClient_global.hpp>
#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

GP_ENUM(GP_DB_CLIENT_API, GpDbExceptionCode,
    CONNECTION_LIMIT_EXCEEDED,
    REQUEST_ERROR,
    RESPONSE_ERROR,
    UNSUPPORTED_FEATURE
);

}// namespace GPlatform

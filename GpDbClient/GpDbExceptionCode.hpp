#pragma once

#include "GpDbClient_global.hpp"

namespace GPlatform {

GP_ENUM(GP_DB_CLIENT_API, GpDbExceptionCode,
    CONNECTION_ERROR,
    CONNECTION_LIMIT_EXCEEDED,
    QUERY_ERROR,
    QUERY_RESULT_COUNT_LOW,
    QUERY_DUPLICATE_KEY,
    ROWS_COUNT_OUT_OF_RANGE
);

}//namespace GPlatform

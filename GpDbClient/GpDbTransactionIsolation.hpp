#pragma once

#include "GpDbClient_global.hpp"

namespace GPlatform {

GP_ENUM(GP_DB_CLIENT_API, GpDbTransactionIsolation,
    SERIALIZABLE,
    REPEATABLE_READ,
    READ_COMMITTED,
    READ_UNCOMMITTED
);

}//namespace GPlatform

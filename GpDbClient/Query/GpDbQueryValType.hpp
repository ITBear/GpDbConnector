#pragma once

#include "../GpDbClient_global.hpp"
#include "../../../GpCore2/GpUtils/Types/Enums/GpEnum.hpp"
#include "../../../GpCore2/GpUtils/Types/Strings/GpStringOps.hpp"
#include "../../../GpCore2/GpReflection/GpReflectType.hpp"

namespace GPlatform {

GP_ENUM(GP_DB_CLIENT_API, GpDbQueryValType,
    INT_16,
    INT_16_ARRAY_1D,
    INT_32,
    INT_32_ARRAY_1D,
    INT_64,
    INT_64_ARRAY_1D,
    DOUBLE,
    DOUBLE_ARRAY_1D,
    FLOAT,
    FLOAT_ARRAY_1D,
    STRING,
    STRING_ARRAY_1D,
    JSON,
    JSON_ARRAY_1D,
    UUID,
    UUID_ARRAY_1D,
    BLOB,
    BLOB_ARRAY_1D,
    BOOLEAN,
    BOOLEAN_ARRAY_1D,
    NULL_VAL
);

class GP_DB_CLIENT_API GpDbQueryValTypeUtils
{
public:
    static GpDbQueryValType::EnumT      SReflectToDbType    (const GpReflectType::EnumT aType);
};

}//namespace GPlatform

#include <GpDbConnector/GpDbClient/Query/GpDbQueryValType.hpp>

namespace GPlatform {

GP_ENUM_IMPL(GpDbQueryValType)

GpDbQueryValType::EnumT GpDbQueryValTypeUtils::SReflectToDbType (const GpReflectType::EnumT aType)
{
    const std::array<GpDbQueryValType::EnumT, GpReflectType::SCount()> s =
    {
        GpDbQueryValType::NULL_VAL,         // GpReflectType::NOT_SET
        GpDbQueryValType::INT_16,           // GpReflectType::U_INT_8
        GpDbQueryValType::INT_16,           // GpReflectType::S_INT_8
        GpDbQueryValType::INT_16,           // GpReflectType::U_INT_16
        GpDbQueryValType::INT_16,           // GpReflectType::S_INT_16
        GpDbQueryValType::INT_32,           // GpReflectType::U_INT_32
        GpDbQueryValType::INT_32,           // GpReflectType::S_INT_32
        GpDbQueryValType::INT_64,           // GpReflectType::U_INT_64
        GpDbQueryValType::INT_64,           // GpReflectType::S_INT_64
        GpDbQueryValType::DOUBLE,           // GpReflectType::DOUBLE
        GpDbQueryValType::FLOAT,            // GpReflectType::FLOAT
        GpDbQueryValType::BOOLEAN,          // GpReflectType::BOOLEAN
        GpDbQueryValType::UUID,             // GpReflectType::UUID
        GpDbQueryValType::STRING,           // GpReflectType::STRING
        GpDbQueryValType::BLOB,             // GpReflectType::BLOB
        GpDbQueryValType::JSON,             // GpReflectType::STRUCT
        GpDbQueryValType::JSON,             // GpReflectType::STRUCT_SP
        GpDbQueryValType::STRING,           // GpReflectType::ENUM
        GpDbQueryValType::STRING_ARRAY_1D   // GpReflectType::ENUM_FLAGS
    };

    return s.at(size_t(aType));
}

}// namespace GPlatform

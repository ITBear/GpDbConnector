#include "GpDbQueryPreparedPgSql.hpp"

namespace GPlatform {

GpDbQueryPreparedPgSql::GpDbQueryPreparedPgSql (void) noexcept
{
}

GpDbQueryPreparedPgSql::~GpDbQueryPreparedPgSql (void) noexcept
{
}

void    GpDbQueryPreparedPgSql::Prepare (const GpDbQuery& aQuery)
{
    const GpDbQuery::TypeVecT& types = aQuery.Types();

    const size_t typesCount     = types.size();
    const size_t valuesCount    = aQuery.Values().size();

    THROW_COND_GP
    (
        typesCount == valuesCount,
        [&aQuery](){return "valuesTypesCount != valuesCount. SQL '"_sv + aQuery.QueryStr() + "'"_sv;}
    );

    iOIDs.reserve(valuesCount);
    iValuesPtr.reserve(valuesCount);
    iValuesSize.reserve(valuesCount);
    iValuesIsBinary.reserve(valuesCount);
    iSInt64Vec.reserve(valuesCount);

    for (size_t id = 0; id < valuesCount; ++id)
    {
        const GpDbQueryValType::EnumT valueType = types.at(id);
        FillData(valueType, id, aQuery);
    }
}

void    GpDbQueryPreparedPgSql::FillData
(
    const GpDbQueryValType::EnumT   aValueType,
    const size_t                    aValueId,
    const GpDbQuery&                aQuery
)
{
    switch (aValueType)
    {
        case GpDbQueryValType::INT_16:
        {
            const s_int_16 value = BitOps::N2H(aQuery.Int16(aValueId));
            iSInt64Vec.emplace_back(0);

            char* ptr = reinterpret_cast<char*>(iSInt64Vec.data() + (iSInt64Vec.size() - 1));
            std::memcpy(ptr, &value, sizeof(decltype(value)));

            iOIDs.emplace_back(0);
            iValuesPtr.emplace_back(ptr);
            iValuesSize.emplace_back(NumOps::SConvert<int>(sizeof(decltype(value))));
            iValuesIsBinary.emplace_back(1);
        } break;
        case GpDbQueryValType::INT_16_ARRAY_1D:
        {
            const std::vector<s_int_16>& value = aQuery.Int16Array1D(aValueId);
            _FillArray(value);
        } break;
        case GpDbQueryValType::INT_32:
        {
            const s_int_32 value = BitOps::N2H(aQuery.Int32(aValueId));
            iSInt64Vec.emplace_back(0);

            char* ptr = reinterpret_cast<char*>(iSInt64Vec.data() + (iSInt64Vec.size() - 1));
            std::memcpy(ptr, &value, sizeof(decltype(value)));

            iOIDs.emplace_back(0);
            iValuesPtr.emplace_back(ptr);
            iValuesSize.emplace_back(NumOps::SConvert<int>(sizeof(decltype(value))));
            iValuesIsBinary.emplace_back(1);
        } break;
        case GpDbQueryValType::INT_32_ARRAY_1D:
        {
            const std::vector<s_int_32>& value = aQuery.Int32Array1D(aValueId);
            _FillArray(value);
        } break;
        case GpDbQueryValType::INT_64:
        {
            const s_int_64 value = BitOps::N2H(aQuery.Int64(aValueId));
            iSInt64Vec.emplace_back(0);

            char* ptr = reinterpret_cast<char*>(iSInt64Vec.data() + (iSInt64Vec.size() - 1));
            std::memcpy(ptr, &value, sizeof(decltype(value)));

            iOIDs.emplace_back(0);
            iValuesPtr.emplace_back(ptr);
            iValuesSize.emplace_back(NumOps::SConvert<int>(sizeof(decltype(value))));
            iValuesIsBinary.emplace_back(1);
        } break;
        case GpDbQueryValType::INT_64_ARRAY_1D:
        {
            const std::vector<s_int_64>& value = aQuery.Int64Array1D(aValueId);
            _FillArray(value);
        } break;
        case GpDbQueryValType::DOUBLE:
        {
            const double value = BitOps::N2H(aQuery.Double(aValueId));
            iSInt64Vec.emplace_back(0);

            char* ptr = reinterpret_cast<char*>(iSInt64Vec.data() + (iSInt64Vec.size() - 1));
            std::memcpy(ptr, &value, sizeof(decltype(value)));

            iOIDs.emplace_back(0);
            iValuesPtr.emplace_back(ptr);
            iValuesSize.emplace_back(NumOps::SConvert<int>(sizeof(decltype(value))));
            iValuesIsBinary.emplace_back(1);
        } break;
        case GpDbQueryValType::DOUBLE_ARRAY_1D:
        {
            const std::vector<double>& value = aQuery.DoubleArray1D(aValueId);
            _FillArray(value);
        } break;
        case GpDbQueryValType::FLOAT:
        {
            const float value = BitOps::N2H(aQuery.Float(aValueId));
            iSInt64Vec.emplace_back(0);

            char* ptr = reinterpret_cast<char*>(iSInt64Vec.data() + (iSInt64Vec.size() - 1));
            std::memcpy(ptr, &value, sizeof(decltype(value)));

            iOIDs.emplace_back(0);
            iValuesPtr.emplace_back(ptr);
            iValuesSize.emplace_back(NumOps::SConvert<int>(sizeof(decltype(value))));
            iValuesIsBinary.emplace_back(1);
        } break;
        case GpDbQueryValType::FLOAT_ARRAY_1D:
        {
            const std::vector<float>& value = aQuery.FloatArray1D(aValueId);
            _FillArray(value);
        } break;
        case GpDbQueryValType::STRING:
        {
            std::string_view value = aQuery.Str(aValueId);

            iOIDs.emplace_back(0);
            iValuesPtr.emplace_back(value.data());
            iValuesSize.emplace_back(NumOps::SConvert<int>(value.size()));
            iValuesIsBinary.emplace_back(1);
        } break;
        case GpDbQueryValType::STRING_ARRAY_1D:
        {
            const std::vector<std::string>& value = aQuery.StrArray1D(aValueId);
            _FillArray(value);
        } break;
        case GpDbQueryValType::JSON:
        {
            std::string_view value = aQuery.Json(aValueId);

            GpBytesArray jsonbData;
            jsonbData.resize(NumOps::SAdd(value.size(), size_t(1)));
            GpByteWriterStorageFixedSize    writerStorage(jsonbData);
            GpByteWriter                    writer(writerStorage);

            writer.UInt8(1);//Jsonb version
            writer.Bytes(value);

            const std::byte*    dataPtr     = jsonbData.data();
            const size_t        dataSize    = jsonbData.size();

            iBinaryDataVec.emplace_back(std::move(jsonbData));

            //---------------------------
            iOIDs.emplace_back(0);
            iValuesPtr.emplace_back(reinterpret_cast<const char*>(dataPtr));
            iValuesSize.emplace_back(NumOps::SConvert<int>(dataSize));
            iValuesIsBinary.emplace_back(1);
        } break;
        case GpDbQueryValType::JSON_ARRAY_1D:
        {
            const std::vector<std::string>& value = aQuery.JsonArray1D(aValueId);
            _FillArray(value);
        } break;
        case GpDbQueryValType::UUID:
        {
            const GpUUID& value = aQuery.Uuid(aValueId);

            if (value.IsNotZero())
            {
                iOIDs.emplace_back(0);
                iValuesPtr.emplace_back(reinterpret_cast<const char*>(value.Data().data()));
                iValuesSize.emplace_back(NumOps::SConvert<int>(sizeof(GpUUID::DataT)));
                iValuesIsBinary.emplace_back(1);
            } else
            {
                iOIDs.emplace_back(0);
                iValuesPtr.emplace_back(nullptr);
                iValuesSize.emplace_back(0);
                iValuesIsBinary.emplace_back(1);
            }
        } break;
        case GpDbQueryValType::UUID_ARRAY_1D:
        {
            const std::vector<GpUUID>& value = aQuery.UuidArray1D(aValueId);
            _FillArray(value);
        } break;
        case GpDbQueryValType::BLOB:
        {
            const GpBytesArray& value = aQuery.Blob(aValueId);

            iOIDs.emplace_back(0);
            iValuesPtr.emplace_back(reinterpret_cast<const char*>(value.data()));
            iValuesSize.emplace_back(NumOps::SConvert<int>(value.size()));
            iValuesIsBinary.emplace_back(1);
        } break;
        case GpDbQueryValType::BLOB_ARRAY_1D:
        {
            const std::vector<GpBytesArray>& value = aQuery.BlobArray1D(aValueId);
            _FillArray(value);
        } break;
        case GpDbQueryValType::BOOLEAN:
        {
            const bool value = aQuery.Boolean(aValueId);
            iSInt64Vec.emplace_back(0);
            char* ptr = reinterpret_cast<char*>(iSInt64Vec.data() + (iSInt64Vec.size() - 1));
            *ptr = (value ? 1: 0);

            iOIDs.emplace_back(0);
            iValuesPtr.emplace_back(ptr);
            iValuesSize.emplace_back(1);
            iValuesIsBinary.emplace_back(1);
        } break;
        case GpDbQueryValType::BOOLEAN_ARRAY_1D:
        {
            const std::vector<bool>& value = aQuery.BooleanArray1D(aValueId);
            _FillArray(value);
        } break;
        case GpDbQueryValType::NULL_VAL:
        {
            iOIDs.emplace_back(0);
            iValuesPtr.emplace_back(nullptr);
            iValuesSize.emplace_back(0);
            iValuesIsBinary.emplace_back(1);
        } break;
        default:
        {
            THROW_GP("Unknown value type"_sv);
        };
    }
}

}//GPlatform

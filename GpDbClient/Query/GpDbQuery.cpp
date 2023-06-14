#include "GpDbQuery.hpp"
#include "GpDbQueryMapperCache.hpp"
#include "../../../GpJson/GpJson.hpp"

namespace GPlatform {

GpDbQuery::GpDbQuery (const GpDbQueryMapperCacheValue& aCacheValue):
iQueryStr(aCacheValue.iQuery),
iTypes(aCacheValue.iTypes)
{
}

GpDbQuery::~GpDbQuery (void) noexcept
{
}

std::u8string   GpDbQuery::ValuesToStr (void) const
{
    const size_t valuesCount = iValues.size();

    std::u8string resStr;
    resStr.reserve(1024);

    for (size_t id = 0; id < valuesCount; ++id)
    {
        const GpDbQueryValue&           value       = iValues.at(id);
        const GpDbQueryValType::EnumT   valueType   = iTypes.at(id);

        if (id > 0)
        {
            resStr.append(u8"\n"_sv);
        }

        resStr.append(u8" ["_sv + (id+1));

        switch (valueType)
        {
            case GpDbQueryValType::INT_16:
            {
                resStr.append(u8" (int16)]:     "_sv).append(StrOps::SToString(std::get<s_int_16>(value)));
            } break;
            case GpDbQueryValType::INT_16_ARRAY_1D:
            {
                std::u8string s = StrOps::SJoin<std::u8string>
                (
                    std::get<std::vector<s_int_16>>(value),
                    [](const auto& i)->std::u8string {return StrOps::SToString(*i);},
                    u8","_sv
                );

                resStr.append(u8" (int16[])]:  ["_sv).append(s).append(u8"]"_sv);
            } break;
            case GpDbQueryValType::INT_32:
            {
                resStr.append(u8" (int32)]:     "_sv).append(StrOps::SToString(std::get<s_int_32>(value)));
            } break;
            case GpDbQueryValType::INT_32_ARRAY_1D:
            {
                std::u8string s = StrOps::SJoin<std::u8string>
                (
                    std::get<std::vector<s_int_32>>(value),
                    [](const auto& i)->std::u8string {return StrOps::SToString(*i);},
                    u8","_sv
                );

                resStr.append(u8" (int32[])]:  ["_sv).append(s).append(u8"]"_sv);
            } break;
            case GpDbQueryValType::INT_64:
            {
                resStr.append(u8" (int64)]:     "_sv).append(StrOps::SToString(std::get<s_int_64>(value)));
            } break;
            case GpDbQueryValType::INT_64_ARRAY_1D:
            {
                std::u8string s = StrOps::SJoin<std::u8string>
                (
                    std::get<std::vector<s_int_64>>(value),
                    [](const auto& i)->std::u8string {return StrOps::SToString(*i);},
                    u8","_sv
                );

                resStr.append(u8" (int64[])]:  ["_sv).append(s).append(u8"]"_sv);
            } break;
            case GpDbQueryValType::DOUBLE:
            {
                resStr.append(u8" (double)]:    "_sv).append(StrOps::SToString(std::get<double>(value)));
            } break;
            case GpDbQueryValType::DOUBLE_ARRAY_1D:
            {
                std::u8string s = StrOps::SJoin<std::u8string>
                (
                    std::get<std::vector<double>>(value),
                    [](const auto& i)->std::u8string {return StrOps::SToString(*i);},
                    u8","_sv
                );

                resStr.append(u8" (double[])]: ["_sv).append(s).append(u8"]"_sv);
            } break;
            case GpDbQueryValType::FLOAT:
            {
                resStr.append(u8" (float)]:     "_sv).append(StrOps::SToString(std::get<float>(value)));
            } break;
            case GpDbQueryValType::FLOAT_ARRAY_1D:
            {
                std::u8string s = StrOps::SJoin<std::u8string>
                (
                    std::get<std::vector<float>>(value),
                    [](const auto& i)->std::u8string {return StrOps::SToString(*i);},
                    u8","_sv
                );

                resStr.append(u8" (float[])]:  ["_sv).append(s).append(u8"]"_sv);
            } break;
            case GpDbQueryValType::STRING:
            {
                resStr.append(u8" (str)]:      '"_sv).append(std::get<std::u8string>(value)).append(u8"'"_sv);
            } break;
            case GpDbQueryValType::STRING_ARRAY_1D:
            {
                std::u8string s = StrOps::SJoin<std::u8string>
                (
                    std::get<std::vector<std::u8string>>(value),
                    [](const auto& i)->std::u8string {return u8"'"_sv + *i + u8"'"_sv;},
                    u8","_sv
                );

                resStr.append(u8" (str[])]:    ["_sv).append(s).append(u8"]"_sv);
            } break;
            case GpDbQueryValType::JSON:
            {
                resStr.append(u8" (json)]:     '"_sv).append(std::get<std::u8string>(value)).append(u8"'"_sv);
            } break;
            case GpDbQueryValType::JSON_ARRAY_1D:
            {
                std::u8string s = StrOps::SJoin<std::u8string_view>
                (
                    std::get<std::vector<std::u8string>>(value),
                    [](const auto& i)->std::u8string {return u8"'"_sv + *i + u8"'"_sv;},
                    u8","_sv
                );

                resStr.append(u8" (json[])]:   ["_sv).append(s).append(u8"]"_sv);
            } break;
            case GpDbQueryValType::UUID:
            {
                resStr.append(u8" (uuid)]:      "_sv).append(std::get<GpUUID>(value).ToString());
            } break;
            case GpDbQueryValType::UUID_ARRAY_1D:
            {
                std::u8string s = StrOps::SJoin<std::u8string>
                (
                    std::get<std::vector<GpUUID>>(value),
                    [](const auto& i)->std::u8string {return i->ToString();},
                    u8","_sv
                );

                resStr.append(u8" (uuid[])]:   ["_sv).append(s).append(u8"]"_sv);
            } break;
            case GpDbQueryValType::BLOB:
            {
                const GpBytesArray& blob = std::get<GpBytesArray>(value);
                resStr.append(u8" (blob)]:      "_sv).append(StrOps::SFromBytesHex(GpSpanPtrByteR(blob.data(), blob.size())));
            } break;
            case GpDbQueryValType::BLOB_ARRAY_1D:
            {
                std::u8string s = StrOps::SJoin<std::u8string>
                (
                    std::get<std::vector<GpBytesArray>>(value),
                    [](const auto& i)->std::u8string
                    {
                        const auto& b = *i;
                        return StrOps::SFromBytesHex(GpSpanPtrByteR(b.data(), b.size()));
                    },
                    u8","_sv
                );

                resStr.append(u8" (blob[])]:   ["_sv).append(s).append(u8"]"_sv);
            } break;
            case GpDbQueryValType::BOOLEAN:
            {
                resStr.append(u8" (bool)]:      "_sv).append(std::get<bool>(value) ? u8"true"_sv : u8"false"_sv);
            } break;
            case GpDbQueryValType::BOOLEAN_ARRAY_1D:
            {
                std::u8string s = StrOps::SJoin<std::u8string_view>
                (
                    std::get<std::vector<bool>>(value),
                    [](const auto& i)->std::u8string_view {return *i ? u8"true"_sv : u8"false"_sv;},
                    u8","_sv
                );

                resStr.append(u8" (bool[])]:   ["_sv).append(s).append(u8"]"_sv);
            } break;
            case GpDbQueryValType::NULL_VAL:
            {
                resStr.append(u8" (null val)]:  null"_sv);
            } break;
            default:
            {
                THROW_GP(u8"Unknown type"_sv);
            }
        }
    }

    return resStr;
}

void    GpDbQuery::SetValues (GpDbQuery&& aDbQuery)
{
    iTypes  = std::move(aDbQuery.iTypes);
    iValues = std::move(aDbQuery.iValues);
}

void    GpDbQuery::AddValues (GpDbQuery&& aDbQuery)
{
    auto& vt    = aDbQuery.iTypes;
    auto& v     = aDbQuery.iValues;

    iTypes.insert
    (
        iTypes.end(),
        std::make_move_iterator(vt.begin()),
        std::make_move_iterator(vt.end())
    );

    iValues.insert
    (
        iValues.end(),
        std::make_move_iterator(v.begin()),
        std::make_move_iterator(v.end())
    );
}

void    GpDbQuery::Reset (void)
{
    iValues.clear();
}

GpDbQuery&  GpDbQuery::NextInt16 (const s_int_16 aValue)
{
    _Next<GpDbQueryValType::INT_16>(-1, aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::NextInt16Array1D (const std::vector<s_int_16>& aValue)
{
    _Next<GpDbQueryValType::INT_16_ARRAY_1D>(-1, _MakeArray<s_int_16>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextInt16Array1D (std::vector<s_int_16>&& aValue)
{
    _Next<GpDbQueryValType::INT_16_ARRAY_1D>(-1, std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::InsertInt16
(
    const size_t    aId,
    const s_int_16  aValue
)
{
    _Next<GpDbQueryValType::INT_32>(NumOps::SConvert<ssize_t>(aId), aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::InsertInt16Array1D
(
    const size_t                    aId,
    const std::vector<s_int_16>&    aValue
)
{
    _Next<GpDbQueryValType::INT_32_ARRAY_1D>(NumOps::SConvert<ssize_t>(aId), _MakeArray<s_int_32>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::InsertInt16Array1D
(
    const size_t            aId,
    std::vector<s_int_16>&& aValue
)
{
    _Next<GpDbQueryValType::INT_32_ARRAY_1D>(NumOps::SConvert<ssize_t>(aId), std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextInt32 (const s_int_32 aValue)
{
    _Next<GpDbQueryValType::INT_32>(-1, aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::NextInt32Array1D (const std::vector<s_int_32>& aValue)
{
    _Next<GpDbQueryValType::INT_32_ARRAY_1D>(-1, _MakeArray<s_int_32>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextInt32Array1D (std::vector<s_int_32>&& aValue)
{
    _Next<GpDbQueryValType::INT_32_ARRAY_1D>(-1, std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::InsertInt32
(
    const size_t    aId,
    const s_int_32  aValue
)
{
    _Next<GpDbQueryValType::INT_32>(NumOps::SConvert<ssize_t>(aId), aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::InsertInt32Array1D
(
    const size_t                    aId,
    const std::vector<s_int_32>&    aValue
)
{
    _Next<GpDbQueryValType::INT_32_ARRAY_1D>(NumOps::SConvert<ssize_t>(aId), _MakeArray<s_int_32>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::InsertInt32Array1D
(
    const size_t            aId,
    std::vector<s_int_32>&& aValue
)
{
    _Next<GpDbQueryValType::INT_32_ARRAY_1D>(NumOps::SConvert<ssize_t>(aId), std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextInt64 (const s_int_64 aValue)
{
    _Next<GpDbQueryValType::INT_64>(-1, aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::NextInt64Array1D (const std::vector<s_int_64>& aValue)
{
    _Next<GpDbQueryValType::INT_64_ARRAY_1D>(-1, _MakeArray<s_int_64>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextInt64Array1D (std::vector<s_int_64>&& aValue)
{
    _Next<GpDbQueryValType::INT_64_ARRAY_1D>(-1, std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextDouble (const double aValue)
{
    _Next<GpDbQueryValType::DOUBLE>(-1, aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::NextDoubleArray1D (const std::vector<double>& aValue)
{
    _Next<GpDbQueryValType::DOUBLE_ARRAY_1D>(-1, _MakeArray<double>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextDoubleArray1D (std::vector<double>&& aValue)
{
    _Next<GpDbQueryValType::DOUBLE_ARRAY_1D>(-1, std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextFloat (const float aValue)
{
    _Next<GpDbQueryValType::FLOAT>(-1, aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::NextFloatArray1D (const std::vector<float>& aValue)
{
    _Next<GpDbQueryValType::FLOAT_ARRAY_1D>(-1, _MakeArray<float>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextFloatArray1D (std::vector<float>&& aValue)
{
    _Next<GpDbQueryValType::FLOAT_ARRAY_1D>(-1, std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStr (std::u8string_view aValue)
{
    _Next<GpDbQueryValType::STRING>(-1, std::u8string(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStr (std::u8string&& aValue)
{
    _Next<GpDbQueryValType::STRING>(-1, std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrArray1D (const std::vector<std::u8string_view>& aValue)
{
    _Next<GpDbQueryValType::STRING_ARRAY_1D>(-1, _MakeArray<std::u8string>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrArray1D (const std::vector<std::u8string>& aValue)
{
    _Next<GpDbQueryValType::STRING_ARRAY_1D>(-1, _MakeArray<std::u8string>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrArray1D (std::vector<std::u8string>&& aValue)
{
    _Next<GpDbQueryValType::STRING_ARRAY_1D>(-1, std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextStrArray1D (const GpEnumFlags& aValue)
{
    _Next<GpDbQueryValType::STRING_ARRAY_1D>(-1, aValue.ToStringArray());
    return *this;
}

GpDbQuery&  GpDbQuery::NextJson (std::u8string_view aValue)
{
    _Next<GpDbQueryValType::JSON>(-1, std::u8string(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextJson (std::u8string&& aValue)
{
    _Next<GpDbQueryValType::JSON>(-1, std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextJson (const GpReflectObject& aValue)
{
    return NextJson(GpJsonSerializer::SToStr(aValue, {GpJsonSerializerFlag::WRITE_MODEL_UID}));
}

GpDbQuery&  GpDbQuery::NextJsonArray1D (const std::vector<std::u8string_view>& aValue)
{
    _Next<GpDbQueryValType::JSON_ARRAY_1D>(-1, _MakeArray<std::u8string>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextJsonArray1D (const std::vector<std::u8string>& aValue)
{
    _Next<GpDbQueryValType::JSON_ARRAY_1D>(-1, _MakeArray<std::u8string>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextJsonArray1D (std::vector<std::u8string>&& aValue)
{
    _Next<GpDbQueryValType::JSON_ARRAY_1D>(-1, std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextJsonArray1D (const std::vector<GpReflectObject::SP>& aValue)
{
    const size_t            size = aValue.size();
    std::vector<std::u8string>  v(size);

    for (size_t id = 0; id < size; id++)
    {
        v[id] = GpJsonSerializer::SToStr(aValue[id].V(), {GpJsonSerializerFlag::WRITE_MODEL_UID});
    }

    _Next<GpDbQueryValType::JSON_ARRAY_1D>(-1, std::move(v));
    return *this;
}

GpDbQuery&  GpDbQuery::NextUuid (const GpUUID& aValue)
{
    _Next<GpDbQueryValType::UUID>(-1, aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::NextUuidArray1D (const std::vector<GpUUID>& aValue)
{
    _Next<GpDbQueryValType::UUID_ARRAY_1D>(-1, _MakeArray<GpUUID>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextUuidArray1D (std::vector<GpUUID>&& aValue)
{
    _Next<GpDbQueryValType::UUID_ARRAY_1D>(-1, std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextBlob (const GpSpanPtrByteR aValue)
{
    _Next<GpDbQueryValType::BLOB>(-1, aValue.ToByteArray());
    return *this;
}

GpDbQuery&  GpDbQuery::NextBlob (GpBytesArray&& aValue)
{
    _Next<GpDbQueryValType::BLOB>(-1, std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextBlobArray1D (const std::vector<GpBytesArray>& aValue)
{
    _Next<GpDbQueryValType::BLOB_ARRAY_1D>(-1, _MakeArray<GpBytesArray>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextBlobArray1D (std::vector<GpBytesArray>&& aValue)
{
    _Next<GpDbQueryValType::BLOB_ARRAY_1D>(-1, std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextBoolean (const bool aValue)
{
    _Next<GpDbQueryValType::BOOLEAN>(-1, aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::NextBooleanArray1D (const std::vector<bool>& aValue)
{
    _Next<GpDbQueryValType::BOOLEAN_ARRAY_1D>(-1, _MakeArray<bool>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextBooleanArray1D (std::vector<bool>&& aValue)
{
    _Next<GpDbQueryValType::BOOLEAN_ARRAY_1D>(-1, std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::NextNULL (void)
{
    _Next<GpDbQueryValType::NULL_VAL>(-1, std::nullopt);
    return *this;
}

GpDbQuery&  GpDbQuery::NextProp
(
    const GpReflectProp&    aProp,
    const void*             aDataPtr
)
{
    const GpReflectContainerType::EnumT propContainer = aProp.Container();

    switch (propContainer)
    {
        case GpReflectContainerType::NO:
        {
            _NextProp(aProp, aDataPtr);
        } break;
        case GpReflectContainerType::VECTOR:
        {
            _NextPropVec(aProp, aDataPtr);
        } break;
        case GpReflectContainerType::MAP:
        {
            _NextPropMap(aProp, aDataPtr);
        } break;
        default:
        {
            THROW_GP(u8"Unknown container type "_sv + GpReflectContainerType::SToString(propContainer));
        }
    }

    return *this;
}

GpDbQuery&  GpDbQuery::AddInt16 (const s_int_16 aValue)
{
    _Add<GpDbQueryValType::INT_16>(aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::AddInt16Array1D (const std::vector<s_int_16>& aValue)
{
    _Add<GpDbQueryValType::INT_16_ARRAY_1D>(_MakeArray<s_int_16>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddInt16Array1D (std::vector<s_int_16>&& aValue)
{
    _Add<GpDbQueryValType::INT_16_ARRAY_1D>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddInt32 (const s_int_32 aValue)
{
    _Add<GpDbQueryValType::INT_32>(aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::AddInt32Array1D (const std::vector<s_int_32>& aValue)
{
    _Add<GpDbQueryValType::INT_32_ARRAY_1D>(_MakeArray<s_int_32>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddInt32Array1D (std::vector<s_int_32>&& aValue)
{
    _Add<GpDbQueryValType::INT_32_ARRAY_1D>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddInt64 (const s_int_64 aValue)
{
    _Add<GpDbQueryValType::INT_64>(aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::AddInt64Array1D (const std::vector<s_int_64>& aValue)
{
    _Add<GpDbQueryValType::INT_64_ARRAY_1D>(_MakeArray<s_int_64>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddInt64Array1D (std::vector<s_int_64>&& aValue)
{
    _Add<GpDbQueryValType::INT_64_ARRAY_1D>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddDouble (const double aValue)
{
    _Add<GpDbQueryValType::DOUBLE>(aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::AddDoubleArray1D (const std::vector<double>& aValue)
{
    _Add<GpDbQueryValType::DOUBLE_ARRAY_1D>(_MakeArray<double>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddDoubleArray1D (std::vector<double>&& aValue)
{
    _Add<GpDbQueryValType::DOUBLE_ARRAY_1D>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddFloat (const float aValue)
{
    _Add<GpDbQueryValType::FLOAT>(aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::AddFloatArray1D (const std::vector<float>& aValue)
{
    _Add<GpDbQueryValType::FLOAT_ARRAY_1D>(_MakeArray<float>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddFloatArray1D (std::vector<float>&& aValue)
{
    _Add<GpDbQueryValType::FLOAT_ARRAY_1D>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddStr (std::u8string_view aValue)
{
    _Add<GpDbQueryValType::STRING>(std::u8string(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddStr (std::u8string&& aValue)
{
    _Add<GpDbQueryValType::STRING>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddStrArray1D (const std::vector<std::u8string_view>& aValue)
{
    _Add<GpDbQueryValType::STRING_ARRAY_1D>(_MakeArray<std::u8string>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddStrArray1D (const std::vector<std::u8string>& aValue)
{
    _Add<GpDbQueryValType::STRING_ARRAY_1D>(_MakeArray<std::u8string>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddStrArray1D (std::vector<std::u8string>&& aValue)
{
    _Add<GpDbQueryValType::STRING_ARRAY_1D>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddStrArray1D (const GpEnumFlags& aValue)
{
    _Add<GpDbQueryValType::STRING_ARRAY_1D>(aValue.ToStringArray());
    return *this;
}

GpDbQuery&  GpDbQuery::AddJson (std::u8string_view aValue)
{
    _Add<GpDbQueryValType::JSON>(std::u8string(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddJson (std::u8string&& aValue)
{
    _Add<GpDbQueryValType::JSON>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddJson (const GpReflectObject& aValue)
{
    return AddJson(GpJsonSerializer::SToStr(aValue, {GpJsonSerializerFlag::WRITE_MODEL_UID}));
}

GpDbQuery&  GpDbQuery::AddJsonArray1D (const std::vector<std::u8string_view>& aValue)
{
    _Add<GpDbQueryValType::JSON_ARRAY_1D>(_MakeArray<std::u8string>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddJsonArray1D (const std::vector<std::u8string>& aValue)
{
    _Add<GpDbQueryValType::JSON_ARRAY_1D>(_MakeArray<std::u8string>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddJsonArray1D (std::vector<std::u8string>&& aValue)
{
    _Add<GpDbQueryValType::JSON_ARRAY_1D>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddJsonArray1D (const std::vector<GpReflectObject::SP>& aValue)
{
    const size_t            size = aValue.size();
    std::vector<std::u8string>  v(size);

    for (size_t id = 0; id < size; id++)
    {
        v[id] = GpJsonSerializer::SToStr(aValue[id].V(), {GpJsonSerializerFlag::WRITE_MODEL_UID});
    }

    _Add<GpDbQueryValType::JSON_ARRAY_1D>(std::move(v));
    return *this;
}

GpDbQuery&  GpDbQuery::AddUuid (const GpUUID& aValue)
{
    _Add<GpDbQueryValType::UUID>(aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::AddUuidArray1D (const std::vector<GpUUID>& aValue)
{
    _Add<GpDbQueryValType::UUID_ARRAY_1D>(_MakeArray<GpUUID>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddUuidArray1D (std::vector<GpUUID>&& aValue)
{
    _Add<GpDbQueryValType::UUID_ARRAY_1D>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddBlob (const GpSpanPtrByteR aValue)
{
    _Add<GpDbQueryValType::BLOB>(aValue.ToByteArray());
    return *this;
}

GpDbQuery&  GpDbQuery::AddBlob (GpBytesArray&& aValue)
{
    _Add<GpDbQueryValType::BLOB>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddBlobArray1D (const std::vector<GpBytesArray>& aValue)
{
    _Add<GpDbQueryValType::BLOB_ARRAY_1D>(_MakeArray<GpBytesArray>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddBlobArray1D (std::vector<GpBytesArray>&& aValue)
{
    _Add<GpDbQueryValType::BLOB_ARRAY_1D>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddBoolean (const bool aValue)
{
    _Add<GpDbQueryValType::BOOLEAN>(aValue);
    return *this;
}

GpDbQuery&  GpDbQuery::AddBooleanArray1D (const std::vector<bool>& aValue)
{
    _Add<GpDbQueryValType::BOOLEAN_ARRAY_1D>(_MakeArray<bool>(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddBooleanArray1D (std::vector<bool>&& aValue)
{
    _Add<GpDbQueryValType::BOOLEAN_ARRAY_1D>(std::move(aValue));
    return *this;
}

GpDbQuery&  GpDbQuery::AddNULL (void)
{
    _Add<GpDbQueryValType::NULL_VAL>(std::nullopt);
    return *this;
}

GpDbQuery&  GpDbQuery::FromObject
(
    const GpReflectObject&      aObject,
    GpDbQueryBuilderMode::EnumT aMode
)
{
    const GpReflectModel&   model   = aObject.ReflectModel();
    const void*             dataPtr = aObject.ReflectDataPtr();

    for (const GpReflectProp& propInfo: model.Props())
    {
        if (aMode == GpDbQueryBuilderMode::CREATE)
        {
            if (propInfo.FlagTest(GpReflectPropFlag::GENERATED_OUTSIDE))
            {
                continue;
            }
        } else if (aMode == GpDbQueryBuilderMode::READ)
        {
            //NOP
        } else if (aMode == GpDbQueryBuilderMode::UPDATE)
        {
            if (   (propInfo.FlagTest(GpReflectPropFlag::GENERATED_ONCE))
                || (propInfo.FlagTest(GpReflectPropFlag::GENERATED_OUTSIDE)))
            {
                continue;
            }
        }

        NextProp(propInfo, dataPtr);
    }

    return *this;
}

void    GpDbQuery::_NextProp
(
    const GpReflectProp&    aProp,
    const void*             aDataPtr
)
{
    switch (aProp.Type())
    {
        case GpReflectType::U_INT_8:
        {
            NextInt16(NumOps::SConvert<s_int_16>(aProp.Value_UInt8(aDataPtr)));
        } break;
        case GpReflectType::S_INT_8:
        {
            NextInt16(NumOps::SConvert<s_int_16>(aProp.Value_SInt8(aDataPtr)));
        } break;
        case GpReflectType::U_INT_16:
        {
            NextInt16(NumOps::SConvert<s_int_16>(aProp.Value_UInt16(aDataPtr)));
        } break;
        case GpReflectType::S_INT_16:
        {
            NextInt16(NumOps::SConvert<s_int_16>(aProp.Value_SInt16(aDataPtr)));
        } break;
        case GpReflectType::U_INT_32:
        {
            NextInt32(NumOps::SConvert<s_int_32>(aProp.Value_UInt32(aDataPtr)));
        } break;
        case GpReflectType::S_INT_32:
        {
            NextInt32(NumOps::SConvert<s_int_32>(aProp.Value_SInt32(aDataPtr)));
        } break;
        case GpReflectType::U_INT_64:
        {
            NextInt64(NumOps::SConvert<s_int_64>(aProp.Value_UInt64(aDataPtr)));
        } break;
        case GpReflectType::S_INT_64:
        {
            NextInt64(NumOps::SConvert<s_int_64>(aProp.Value_SInt64(aDataPtr)));
        } break;
        case GpReflectType::DOUBLE:
        {
            NextDouble(aProp.Value_Double(aDataPtr));
        } break;
        case GpReflectType::FLOAT:
        {
            NextFloat(aProp.Value_Float(aDataPtr));
        } break;
        case GpReflectType::BOOLEAN:
        {
            NextBoolean(aProp.Value_Bool(aDataPtr));
        } break;
        case GpReflectType::UUID:
        {
            NextUuid(aProp.Value_UUID(aDataPtr));
        } break;
        case GpReflectType::STRING:
        {
            NextStr(aProp.Value_String(aDataPtr));
        } break;
        case GpReflectType::BLOB:
        {
            const GpBytesArray& blob = aProp.Value_BLOB(aDataPtr);
            NextBlob(GpSpanPtrByteR(blob.data(), blob.size()));
        } break;
        case GpReflectType::OBJECT:
        {
            NextJson(GpJsonSerializer::SToStr(aProp.Value_Object(aDataPtr), {GpJsonSerializerFlag::WRITE_MODEL_UID}));
        } break;
        case GpReflectType::OBJECT_SP:
        {
            const auto& objectSP = aProp.Value_ObjectSP(aDataPtr);
            if (objectSP.IsNULL()) NextNULL();
            else NextJson(GpJsonSerializer::SToStr(objectSP.Vn(), {GpJsonSerializerFlag::WRITE_MODEL_UID}));
        } break;
        case GpReflectType::ENUM:
        {
            NextStr(aProp.Value_Enum(aDataPtr).ToString());
        } break;
        case GpReflectType::ENUM_FLAGS:
        {
            NextStrArray1D(aProp.Value_EnumFlags(aDataPtr));
        } break;
        case GpReflectType::NOT_SET:[[fallthrough]];
        default:
        {
            THROW_GP(u8"Unsupported type NOT_SET"_sv); break;
        }
    }
}

void    GpDbQuery::_NextPropVec
(
    const GpReflectProp&    aProp,
    const void*             aDataPtr
)
{
    switch (aProp.Type())
    {
        case GpReflectType::U_INT_8:
        {
            NextInt16Array1D(_MakeNumArray<s_int_16>(aProp.Vec_UInt8(aDataPtr)));
        } break;
        case GpReflectType::S_INT_8:
        {
            NextInt16Array1D(_MakeNumArray<s_int_16>(aProp.Vec_SInt8(aDataPtr)));
        } break;
        case GpReflectType::U_INT_16:
        {
            NextInt16Array1D(_MakeNumArray<s_int_16>(aProp.Vec_UInt16(aDataPtr)));
        } break;
        case GpReflectType::S_INT_16:
        {
            NextInt16Array1D(aProp.Vec_SInt16(aDataPtr));
        } break;
        case GpReflectType::U_INT_32:
        {
            NextInt32Array1D(_MakeNumArray<s_int_32>(aProp.Vec_UInt32(aDataPtr)));
        } break;
        case GpReflectType::S_INT_32:
        {
            NextInt32Array1D(aProp.Vec_SInt32(aDataPtr));
        } break;
        case GpReflectType::U_INT_64:
        {
            NextInt64Array1D(_MakeNumArray<s_int_64>(aProp.Vec_UInt64(aDataPtr)));
        } break;
        case GpReflectType::S_INT_64:
        {
            NextInt64Array1D(aProp.Vec_SInt64(aDataPtr));
        } break;
        case GpReflectType::DOUBLE:
        {
            NextDoubleArray1D(aProp.Vec_Double(aDataPtr));
        } break;
        case GpReflectType::FLOAT:
        {
            NextFloatArray1D(aProp.Vec_Float(aDataPtr));
        } break;
        case GpReflectType::BOOLEAN:
        {
            NextBooleanArray1D(aProp.Vec_Bool(aDataPtr));
        } break;
        case GpReflectType::UUID:
        {
            NextUuidArray1D(aProp.Vec_UUID(aDataPtr));
        } break;
        case GpReflectType::STRING:
        {
            NextStrArray1D(aProp.Vec_String(aDataPtr));
        } break;
        case GpReflectType::BLOB:
        {
            NextBlobArray1D(aProp.Vec_BLOB(aDataPtr));
        } break;
        case GpReflectType::OBJECT:
        {
            THROW_GP(u8"Unsupported type Object vector"_sv); break;
        } break;
        case GpReflectType::OBJECT_SP:
        {
            const std::vector<GpReflectObject::SP>& objectVec = aProp.Vec_ObjectSP(aDataPtr);
            NextJsonArray1D(objectVec);
        } break;
        case GpReflectType::ENUM:
        {
            THROW_GP(u8"Unsupported type ENUM vector"_sv); break;
        } break;
        case GpReflectType::ENUM_FLAGS:
        {
            THROW_GP(u8"Unsupported type ENUM_FLAGS vector"_sv); break;
        } break;
        case GpReflectType::NOT_SET:[[fallthrough]];
        default:
        {
            THROW_GP(u8"Unsupported type NOT_SET"_sv); break;
        }
    }
}

void    GpDbQuery::_NextPropMap
(
    const GpReflectProp&    /*aProp*/,
    const void*             /*aDataPtr*/
)
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

}//namespace GPlatform

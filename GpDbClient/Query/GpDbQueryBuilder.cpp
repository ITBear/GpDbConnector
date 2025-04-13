#include <GpDbConnector/GpDbClient/Query/GpDbQueryBuilder.hpp>
#include <GpDbConnector/GpDbClient/Query/GpDbQuerySearchBuilder.hpp>
#include <GpDbConnector/GpDbClient/GpDbException.hpp>

namespace GPlatform {

template<typename T>
void    GpDbQueryBuilder::Next
(
    const ssize_t   aId,
    auto&&          aValue
)
{
    if constexpr (std::is_move_assignable_v<decltype(aValue)>)
    {
        if (aId < 0)
        {
            iValues.emplace_back(std::move(aValue));
        } else
        {
            iValues.insert(std::begin(iValues) + aId, std::move(aValue));
        }
    } else
    {
        if (aId < 0)
        {
            iValues.emplace_back(aValue);
        } else
        {
            iValues.insert(std::begin(iValues) + aId, aValue);
        }
    }
}

GpDbQueryBuilder::GpDbQueryBuilder (void) noexcept
{
}

GpDbQueryBuilder::GpDbQueryBuilder (GpDbQueryBuilder&& aBuilder) noexcept:
iValues{std::move(aBuilder.iValues)}
{
}

GpDbQueryBuilder::~GpDbQueryBuilder (void) noexcept
{
}

GpDbQueryBuilder&   GpDbQueryBuilder::operator= (GpDbQueryBuilder&& aBuilder) noexcept
{
    iValues = std::move(aBuilder.iValues);
    return *this;
}

GpDbQuery   GpDbQueryBuilder::Build (void)
{
    return GpDbQuery
    {
        std::move(iName),
        std::move(iQuery),
        std::move(iValues)
    };
}

GpDbQueryBuilder&   GpDbQueryBuilder::Name (std::string_view aName)
{
    iName = aName;
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Name (std::string&& aName)
{
    iName = std::move(aName);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Query (const char* aQuery)
{
    iQuery = aQuery;
    ParseQuery();
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Query (std::string_view aQuery)
{
    iQuery = aQuery;
    ParseQuery();
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Query (std::string&& aQuery)
{
    iQuery = std::move(aQuery);
    ParseQuery();
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const s_int_16 aValue)
{
    Next<s_int_16>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const std::vector<s_int_16>& aValue)
{
    Next<std::vector<s_int_16>>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (std::vector<s_int_16>&& aValue)
{
    Next<std::vector<s_int_16>>(-1, std::move(aValue));
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const s_int_32 aValue)
{
    Next<s_int_32>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const std::vector<s_int_32>& aValue)
{
    Next<std::vector<s_int_32>>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (std::vector<s_int_32>&& aValue)
{
    Next<std::vector<s_int_32>>(-1, std::move(aValue));
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const s_int_64 aValue)
{
    Next<s_int_64>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const std::vector<s_int_64>& aValue)
{
    Next<std::vector<s_int_64>>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (std::vector<s_int_64>&& aValue)
{
    Next<std::vector<s_int_64>>(-1, std::move(aValue));
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const double aValue)
{
    Next<double>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const std::vector<double>& aValue)
{
    Next<std::vector<double>>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (std::vector<double>&& aValue)
{
    Next<std::vector<double>>(-1, std::move(aValue));
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const float aValue)
{
    Next<float>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const std::vector<float>& aValue)
{
    Next<std::vector<float>>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (std::vector<float>&& aValue)
{
    Next<std::vector<float>>(-1, std::move(aValue));
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (std::string_view aValue)
{
    Next<std::string>(-1, std::string{aValue});
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (std::string&& aValue)
{
    Next<std::string>(-1, std::move(aValue));
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const std::vector<std::string_view>& aValue)
{
    std::vector<std::string> vec;
    vec.resize(std::size(aValue));
    auto* dataPtr = vec.data();

    for (std::string_view sv: aValue)
    {
        *dataPtr++ = sv;
    }

    Next<std::vector<std::string>>(-1, std::move(vec));
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const std::vector<std::string>& aValue)
{
    Next<std::vector<std::string>>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (std::vector<std::string>&& aValue)
{
    Next<std::vector<std::string>>(-1, std::move(aValue));
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (GpDbQueryValueJsonSv aValue)
{
    Next<GpDbQueryValueJson>(-1, GpDbQueryValueJson{aValue.Value()});
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (GpDbQueryValueJson&& aValue)
{
    Next<GpDbQueryValueJson>(-1, std::move(aValue));
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const GpDbQueryValueJsonSvVec& aValue)
{
    GpDbQueryValueJsonVec       vecWrap;
    std::vector<std::string>&   vec = vecWrap.Value();

    vec.resize(std::size(aValue.Value()));
    auto* dataPtr = vec.data();

    for (std::string_view sv: aValue.Value())
    {
        *dataPtr++ = sv;
    }

    Next<GpDbQueryValueJsonVec>(-1, std::move(vecWrap));
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const GpDbQueryValueJsonVec& aValue)
{
    Next<GpDbQueryValueJsonVec>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (GpDbQueryValueJsonVec&& aValue)
{
    Next<GpDbQueryValueJsonVec>(-1, std::move(aValue));
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const GpUUID& aValue)
{
    Next<GpUUID>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const std::vector<GpUUID>& aValue)
{
    Next<std::vector<GpUUID>>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (std::vector<GpUUID>&& aValue)
{
    Next<std::vector<GpUUID>>(-1, std::move(aValue));
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const GpSpanByteR aValue)
{
    Next<GpBytesArray>(-1, GpArrayUtils::SMake<GpBytesArray>(aValue));
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (GpBytesArray&& aValue)
{
    Next<GpBytesArray>(-1, std::move(aValue));
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (const std::vector<GpBytesArray>& aValue)
{
    Next<std::vector<GpBytesArray>>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (std::vector<GpBytesArray>&& aValue)
{
    Next<std::vector<GpBytesArray>>(-1, std::move(aValue));
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Next (bool aValue)
{
    Next<bool>(-1, aValue);
    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::Null (void)
{
    Next<std::nullopt_t>(-1, std::nullopt);
    return *this;
}

void    GpDbQueryBuilder::ParseQuery (void)
{
    VERIFY
    (
        !iQuery.empty(),
        GpDbExceptionCode::REQUEST_ERROR,
        "SQL string is empty"
    );

    // Count $N values and max(Ni)
    const char*         strPtr  = iQuery.data();
    const char* const   endPtr  = strPtr;
    s_int_64            maxN    = std::numeric_limits<s_int_64>::min();
    size_t              countN  = 0;

    while (strPtr <= endPtr)
    {
        const char ch = *strPtr++;

        if (ch == '$')
        {
            size_t  readCount       = 0;
            ssize_t strRemainSize   = ssize_t(endPtr - strPtr) + 1;

            if (strRemainSize > 0)
            {
                const s_int_64 n = StrOps::SToSI64
                (
                    std::string_view{strPtr, size_t(strRemainSize)},
                    readCount
                );

                if (readCount > 0)
                {
                    countN++;
                    maxN = std::max(maxN, n);
                }
            }
        }
    }

    if (countN > 0)
    {
        VERIFY
        (
            countN <= size_t(maxN),
            GpDbExceptionCode::REQUEST_ERROR,
            [&]()
            {
                return fmt::format
                (
                    "Incorrect SQL parameter binding: '{}'",
                    iQuery
                );
            }
        );

        iValues.reserve(size_t(maxN));
    }
}

/*
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
        case GpReflectContainerType::VECTOR_WRAP:
        {
            _NextPropVecWrap(aProp, aDataPtr);
        } break;
        case GpReflectContainerType::MAP:
        {
            _NextPropMap(aProp, aDataPtr);
        } break;
        default:
        {
            THROW("Unknown container type "_sv + GpReflectContainerType::SToString(propContainer));
        }
    }

    return *this;
}

GpDbQuery&  GpDbQuery::FromObject
(
    const GpReflectObject&      aObject,
    GpDbQueryBuilderMode::EnumT aMode
)
{
    GpReflectModel::CSP     modelCSP    = aObject.ReflectModel();
    const GpReflectModel&   model       = modelCSP.Vn();
    const void*             dataPtr     = aObject.ReflectDataPtr();

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
            // NOP

            //if (propInfo.FlagTest(GpReflectPropFlag::MULTILANGUAGE_STRING))
            //{
            //  propName = "language.get_item("_sv + propName + ") AS "_sv + srcPropName;
            //}
        } else if (aMode == GpDbQueryBuilderMode::UPDATE)
        {
            if (   (propInfo.FlagTest(GpReflectPropFlag::GENERATED_ONCE))
                || (propInfo.FlagTest(GpReflectPropFlag::GENERATED_OUTSIDE))
                || (propInfo.FlagTest(GpReflectPropFlag::PRIMARY_KEY)))
            {
                continue;
            }
        }

        NextProp(propInfo, dataPtr);
    }

    return *this;
}
*/

/*
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
            NextInt16(NumOps::SConvert<s_int_16>(aProp.Value_UI8(aDataPtr)));
        } break;
        case GpReflectType::S_INT_8:
        {
            NextInt16(NumOps::SConvert<s_int_16>(aProp.Value_SI8(aDataPtr)));
        } break;
        case GpReflectType::U_INT_16:
        {
            NextInt16(NumOps::SConvert<s_int_16>(aProp.Value_UI16(aDataPtr)));
        } break;
        case GpReflectType::S_INT_16:
        {
            NextInt16(NumOps::SConvert<s_int_16>(aProp.Value_SI16(aDataPtr)));
        } break;
        case GpReflectType::U_INT_32:
        {
            NextInt32(NumOps::SConvert<s_int_32>(aProp.Value_UI32(aDataPtr)));
        } break;
        case GpReflectType::S_INT_32:
        {
            NextInt32(NumOps::SConvert<s_int_32>(aProp.Value_SI32(aDataPtr)));
        } break;
        case GpReflectType::U_INT_64:
        {
            NextInt64(NumOps::SConvert<s_int_64>(aProp.Value_UI64(aDataPtr)));
        } break;
        case GpReflectType::S_INT_64:
        {
            NextInt64(NumOps::SConvert<s_int_64>(aProp.Value_SI64(aDataPtr)));
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
            NextBlob(GpSpanByteR(std::data(blob), std::size(blob)));
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
            THROW("Unsupported type NOT_SET"_sv); break;
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
            NextInt16Array1D(_MakeNumArray<s_int_16>(aProp.Vec_UI8(aDataPtr)));
        } break;
        case GpReflectType::S_INT_8:
        {
            NextInt16Array1D(_MakeNumArray<s_int_16>(aProp.Vec_SI8(aDataPtr)));
        } break;
        case GpReflectType::U_INT_16:
        {
            NextInt16Array1D(_MakeNumArray<s_int_16>(aProp.Vec_UI16(aDataPtr)));
        } break;
        case GpReflectType::S_INT_16:
        {
            NextInt16Array1D(aProp.Vec_SI16(aDataPtr));
        } break;
        case GpReflectType::U_INT_32:
        {
            NextInt32Array1D(_MakeNumArray<s_int_32>(aProp.Vec_UI32(aDataPtr)));
        } break;
        case GpReflectType::S_INT_32:
        {
            NextInt32Array1D(aProp.Vec_SI32(aDataPtr));
        } break;
        case GpReflectType::U_INT_64:
        {
            NextInt64Array1D(_MakeNumArray<s_int_64>(aProp.Vec_UI64(aDataPtr)));
        } break;
        case GpReflectType::S_INT_64:
        {
            NextInt64Array1D(aProp.Vec_SI64(aDataPtr));
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
            THROW("Unsupported type booleans vector"_sv);
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
            THROW("Unsupported type Object vector"_sv);
        } break;
        case GpReflectType::OBJECT_SP:
        {
            const std::vector<GpReflectObject::SP>& objectVec = aProp.Vec_ObjectSP(aDataPtr);
            NextJsonArray1D(objectVec);
        } break;
        case GpReflectType::ENUM:
        {
            THROW("Unsupported type ENUM vector"_sv);
        } break;
        case GpReflectType::ENUM_FLAGS:
        {
            THROW("Unsupported type ENUM_FLAGS vector"_sv);
        } break;
        case GpReflectType::NOT_SET:[[fallthrough]];
        default:
        {
            THROW("Unsupported type NOT_SET"_sv);
        }
    }
}

void    GpDbQuery::_NextPropVecWrap
(
    const GpReflectProp&    aProp,
    const void*             aDataPtr
)
{
    switch (aProp.Type())
    {
        case GpReflectType::U_INT_8:
        {
            THROW("Unsupported type U_INT_8"_sv);
        } break;
        case GpReflectType::S_INT_8:
        {
            THROW("Unsupported type S_INT_8"_sv);
        } break;
        case GpReflectType::U_INT_16:
        {
            THROW("Unsupported type U_INT_16"_sv);
        } break;
        case GpReflectType::S_INT_16:
        {
            THROW("Unsupported type S_INT_16"_sv);
        } break;
        case GpReflectType::U_INT_32:
        {
            THROW("Unsupported type U_INT_32"_sv);
        } break;
        case GpReflectType::S_INT_32:
        {
            THROW("Unsupported type S_INT_32"_sv);
        } break;
        case GpReflectType::U_INT_64:
        {
            THROW("Unsupported type U_INT_64"_sv);
        } break;
        case GpReflectType::S_INT_64:
        {
            THROW("Unsupported type S_INT_64"_sv);
        } break;
        case GpReflectType::DOUBLE:
        {
            THROW("Unsupported type DOUBLE"_sv);
        } break;
        case GpReflectType::FLOAT:
        {
            THROW("Unsupported type FLOAT"_sv);
        } break;
        case GpReflectType::BOOLEAN:
        {
            THROW("Unsupported type BOOLEAN"_sv);
        } break;
        case GpReflectType::UUID:
        {
            THROW("Unsupported type UUID"_sv);
        } break;
        case GpReflectType::STRING:
        {
            THROW("Unsupported type STRING"_sv);
        } break;
        case GpReflectType::BLOB:
        {
            THROW("Unsupported type BLOB"_sv);
        } break;
        case GpReflectType::OBJECT:
        {
            const GpVectorReflectObjWrapBase& objectVecWrap = aProp.VecWrap_Object(aDataPtr);
            NextJsonArray1D(objectVecWrap);
        } break;
        case GpReflectType::OBJECT_SP:
        {
            THROW("Unsupported type OBJECT_SP"_sv);
        } break;
        case GpReflectType::ENUM:
        {
            THROW("Unsupported type ENUM vector"_sv);
        } break;
        case GpReflectType::ENUM_FLAGS:
        {
            THROW("Unsupported type ENUM_FLAGS vector"_sv);
        } break;
        case GpReflectType::NOT_SET:[[fallthrough]];
        default:
        {
            THROW("Unsupported type NOT_SET"_sv);
        }
    }
}
*/

}// namespace GPlatform

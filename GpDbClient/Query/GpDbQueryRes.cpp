#include "GpDbQueryRes.hpp"
#include "../../GpJson/GpJson.hpp"

namespace GPlatform {

GpReflectObject::SP GpDbQueryRes::GetObject
(
    const size_t aRowId,
    const size_t aColId
) const
{
    std::string_view jsonStr = GetJson(aRowId, aColId, std::nullopt);
    return GpJsonSerializer::SFromStr(jsonStr, {});
}

GpReflectObject::C::Vec::SP GpDbQueryRes::GetObjectArray1D
(
    const size_t    aRowId,
    const size_t    aColId
) const
{
    GpReflectObject::C::Vec::SP         res;
    const std::vector<std::string_view> jsonStrs = GetJsonArray1D(aRowId, aColId, std::nullopt);
    res.reserve(jsonStrs.size());

    for (std::string_view jsonStr: jsonStrs)
    {
        res.emplace_back(GpJsonSerializer::SFromStr(jsonStr, {}));
    }

    return res;
}

GpReflectObject::SP GpDbQueryRes::GetObject
(
    const size_t            aRowId,
    const size_t            aColId,
    const GpReflectModel&   aModel
) const
{
    std::string_view jsonStr = GetJson(aRowId, aColId, std::nullopt);
    return GpJsonSerializer::SFromStr(jsonStr, aModel, {});
}

GpReflectObject::C::Vec::SP GpDbQueryRes::GetObjectArray1D
(
    const size_t            aRowId,
    const size_t            aColId,
    const GpReflectModel&   aModel
) const
{
    GpReflectObject::C::Vec::SP         res;
    const std::vector<std::string_view> jsonStrs = GetJsonArray1D(aRowId, aColId, std::nullopt);
    res.reserve(jsonStrs.size());

    for (std::string_view jsonStr: jsonStrs)
    {
        res.emplace_back(GpJsonSerializer::SFromStr(jsonStr, aModel, {}));
    }

    return res;
}

GpReflectObject::SP GpDbQueryRes::RowToObject
(
    const size_t            aRowId,
    const size_t            aColStartId,
    const GpReflectModel&   aModel
) const
{
    GpReflectObject::SP objectSP    = aModel.NewInstance();
    void*               dataPtr     = objectSP.V().ReflectDataPtr();

    size_t colId = aColStartId;

    for (const GpReflectProp& propInfo: aModel.Props())
    {
        const GpReflectContainerType::EnumT propContainer = propInfo.Container();

        switch (propContainer)
        {
            case GpReflectContainerType::NO:
            {
                _RowToObjectProp(dataPtr, propInfo, aRowId, colId);
            } break;
            case GpReflectContainerType::VECTOR:
            {
                _RowToObjectPropVec(dataPtr, propInfo, aRowId, colId);
            } break;
            case GpReflectContainerType::MAP:
            {
                _RowToObjectPropMap(dataPtr, propInfo, aRowId, colId);
            } break;
            default:
            {
                THROW_GP("Unknown container type "_sv + GpReflectContainerType::SToString(propContainer));
            }
        }

        colId = NumOps::SInc(colId);
    }

    return objectSP;
}

GpReflectObject::C::Vec::SP GpDbQueryRes::RowsToObject
(
    const size_t            aColStartId,
    const GpReflectModel&   aModel
) const
{
    const size_t                            rowsCount = RowsCount();
    typename GpReflectObject::C::Vec::SP    res;

    res.reserve(rowsCount);

    for (size_t rowId = 0; rowId < rowsCount; rowId++)
    {
        res.emplace_back(RowToObject(rowId, aColStartId, aModel));
    }

    return res;
}

void    GpDbQueryRes::_RowToObjectProp
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    const size_t            aRowId,
    const size_t            aColId
) const
{
    switch (aProp.Type())
    {
        case GpReflectType::U_INT_8:
        {
            aProp.Value_UInt8(aDataPtr) = NumOps::SConvert<u_int_8>(GetInt16(aRowId, aColId, s_int_16(0)));
        } break;
        case GpReflectType::S_INT_8:
        {
            aProp.Value_SInt8(aDataPtr) = NumOps::SConvert<s_int_8>(GetInt16(aRowId, aColId, s_int_16(0)));
        } break;
        case GpReflectType::U_INT_16:
        {
            aProp.Value_UInt16(aDataPtr) = NumOps::SConvert<u_int_16>(GetInt16(aRowId, aColId, s_int_16(0)));
        } break;
        case GpReflectType::S_INT_16:
        {
            aProp.Value_SInt16(aDataPtr) = NumOps::SConvert<s_int_16>(GetInt16(aRowId, aColId, s_int_16(0)));
        } break;
        case GpReflectType::U_INT_32:
        {
            aProp.Value_UInt32(aDataPtr) = NumOps::SConvert<u_int_32>(GetInt32(aRowId, aColId, s_int_32(0)));
        } break;
        case GpReflectType::S_INT_32:
        {
            aProp.Value_SInt32(aDataPtr) = NumOps::SConvert<s_int_32>(GetInt32(aRowId, aColId, s_int_32(0)));
        } break;
        case GpReflectType::U_INT_64:
        {
            aProp.Value_UInt64(aDataPtr) = NumOps::SConvert<u_int_64>(GetInt64(aRowId, aColId, s_int_64(0)));
        } break;
        case GpReflectType::S_INT_64:
        {
            aProp.Value_SInt64(aDataPtr) = NumOps::SConvert<s_int_64>(GetInt64(aRowId, aColId, s_int_64(0)));
        } break;
        case GpReflectType::DOUBLE:
        {
            aProp.Value_Double(aDataPtr) = GetDouble(aRowId, aColId, 0.0);
        } break;
        case GpReflectType::FLOAT:
        {
            aProp.Value_Float(aDataPtr) = GetFloat(aRowId, aColId, 0.0);
        } break;
        case GpReflectType::BOOLEAN:
        {
            aProp.Value_Bool(aDataPtr) = GetBoolean(aRowId, aColId, false);
        } break;
        case GpReflectType::UUID:
        {
            aProp.Value_UUID(aDataPtr) = GetUuid(aRowId, aColId, GpUUID());
        } break;
        case GpReflectType::STRING:
        {
            aProp.Value_String(aDataPtr) = GetStr(aRowId, aColId, ""_sv);
        } break;
        case GpReflectType::BLOB:
        {
            aProp.Value_BLOB(aDataPtr) = GetBlob(aRowId, aColId, {}).ToByteArray();
        } break;
        case GpReflectType::OBJECT:
        {
            std::string_view    jsonStr     = GetJson(aRowId, aColId, ""_sv);
            auto&               objectVal   = aProp.Value_Object(aDataPtr);

            if (jsonStr.length() > 0)
            {
                GpJsonSerializer::SFromStr(jsonStr, objectVal, {});
            }
        } break;
        case GpReflectType::OBJECT_SP:
        {
            std::string_view    jsonStr     = GetJson(aRowId, aColId, ""_sv);
            auto&               objectVal   = aProp.Value_ObjectSP(aDataPtr);

            if (jsonStr.length() > 0)
            {
                const GpReflectModel& model = GpReflectManager::S().Find(aProp.ModelUid());
                objectVal = GpJsonSerializer::SFromStr(jsonStr, model, {});
            } else
            {
                objectVal.Clear();
            }
        } break;
        case GpReflectType::ENUM:
        {
            aProp.Value_Enum(aDataPtr).FromString(GetStr(aRowId, aColId, std::nullopt));
        } break;
        case GpReflectType::ENUM_FLAGS:
        {
            aProp.Value_EnumFlags(aDataPtr).FromStringViewArray(GetStrArray1D(aRowId, aColId, std::nullopt));
        } break;
        case GpReflectType::NOT_SET:[[fallthrough]];
        default:
        {
            THROW_GP("Unsupported type NOT_SET"_sv); break;
        }
    }
}

void    GpDbQueryRes::_RowToObjectPropVec
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    const size_t            aRowId,
    const size_t            aColId
) const
{
    switch (aProp.Type())
    {
        case GpReflectType::U_INT_8:
        {
            aProp.Vec_UInt8(aDataPtr) = _SConvertArrayNum<u_int_8>(GetInt16Array1D(aRowId, aColId, {}));
        } break;
        case GpReflectType::S_INT_8:
        {
            aProp.Vec_SInt8(aDataPtr) = _SConvertArrayNum<s_int_8>(GetInt16Array1D(aRowId, aColId, {}));
        } break;
        case GpReflectType::U_INT_16:
        {
            aProp.Vec_UInt16(aDataPtr) = _SConvertArrayNum<u_int_16>(GetInt16Array1D(aRowId, aColId, {}));
        } break;
        case GpReflectType::S_INT_16:
        {
            aProp.Vec_SInt16(aDataPtr) = GetInt16Array1D(aRowId, aColId, {});
        } break;
        case GpReflectType::U_INT_32:
        {
            aProp.Vec_UInt32(aDataPtr) = _SConvertArrayNum<u_int_32>(GetInt32Array1D(aRowId, aColId, {}));
        } break;
        case GpReflectType::S_INT_32:
        {
            aProp.Vec_SInt32(aDataPtr) = GetInt32Array1D(aRowId, aColId, {});
        } break;
        case GpReflectType::U_INT_64:
        {
            aProp.Vec_UInt64(aDataPtr) = _SConvertArrayNum<u_int_64>(GetInt64Array1D(aRowId, aColId, {}));
        } break;
        case GpReflectType::S_INT_64:
        {
            aProp.Vec_SInt64(aDataPtr) = GetInt64Array1D(aRowId, aColId, {});
        } break;
        case GpReflectType::DOUBLE:
        {
            aProp.Vec_Double(aDataPtr) = GetDoubleArray1D(aRowId, aColId, {});
        } break;
        case GpReflectType::FLOAT:
        {
            aProp.Vec_Float(aDataPtr) = GetFloatArray1D(aRowId, aColId, {});
        } break;
        case GpReflectType::BOOLEAN:
        {
            aProp.Vec_Bool(aDataPtr) = GetBooleanArray1D(aRowId, aColId, {});
        } break;
        case GpReflectType::UUID:
        {
            aProp.Vec_UUID(aDataPtr) = GetUuidArray1D(aRowId, aColId, {});
        } break;
        case GpReflectType::STRING:
        {
            std::vector<std::string_view> v = GetStrArray1D(aRowId, aColId, {});
            aProp.Vec_String(aDataPtr) = _SConvertArrayBytes<std::string>(v);
        } break;
        case GpReflectType::BLOB:
        {
            std::vector<GpSpanPtrByteR> v = GetBlobArray1D(aRowId, aColId, {});
            aProp.Vec_BLOB(aDataPtr) = _SConvertArrayBytes<GpBytesArray>(v);
        } break;
        case GpReflectType::OBJECT:
        {
            THROW_GP("Unsupported type Object vector"_sv); break;
        } break;
        case GpReflectType::OBJECT_SP:
        {
             aProp.Vec_ObjectSP(aDataPtr) = GetObjectArray1D(aRowId, aColId, {});
        } break;
        case GpReflectType::ENUM:
        {
            THROW_GP("Unsupported type ENUM vector"_sv); break;
        } break;
        case GpReflectType::ENUM_FLAGS:
        {
            THROW_GP("Unsupported type ENUM_FLAGS vector"_sv); break;
        } break;
        case GpReflectType::NOT_SET:[[fallthrough]];
        default:
        {
            THROW_GP("Unsupported type NOT_SET"_sv); break;
        }
    }
}

void    GpDbQueryRes::_RowToObjectPropMap
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    const size_t            /*aRowId*/,
    const size_t            /*aColId*/
) const
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

}//namespace GPlatform

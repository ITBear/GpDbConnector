#include <GpDbConnector/GpDbClient/Query/GpDbQueryRes.hpp>
#include <GpJson/GpJsonSerializer.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>

namespace GPlatform {

GpReflectObject::SP GpDbQueryRes::ColToObject
(
    const size_t aRowId,
    const size_t aColId
) const
{
    std::string_view jsonStr = GetJson(aRowId, aColId, std::nullopt);
    return GpJsonSerializer::SFromStr(jsonStr, {});
}

GpReflectObject::C::Vec::SP GpDbQueryRes::ColToObjectArray1D
(
    const size_t    aRowId,
    const size_t    aColId
) const
{
    GpReflectObject::C::Vec::SP         res;
    const std::vector<std::string_view> jsonStrs = GetJsonArray1D(aRowId, aColId, std::nullopt);
    res.reserve(std::size(jsonStrs));

    for (std::string_view jsonStr: jsonStrs)
    {
        res.emplace_back(GpJsonSerializer::SFromStr(jsonStr, {}));
    }

    return res;
}

GpReflectObject::SP GpDbQueryRes::ColToObject
(
    const size_t            aRowId,
    const size_t            aColId,
    const GpReflectModel&   aModel
) const
{
    std::string_view jsonStr = GetJson(aRowId, aColId, std::nullopt);
    return GpJsonSerializer::SFromStr(jsonStr, aModel, {});
}

GpReflectObject::C::Vec::SP GpDbQueryRes::ColToObjectArray1D
(
    const size_t            aRowId,
    const size_t            aColId,
    const GpReflectModel&   aModel
) const
{
    GpReflectObject::C::Vec::SP             res;
    const std::vector<std::string_view> jsonStrs = GetJsonArray1D(aRowId, aColId, std::nullopt);
    res.reserve(std::size(jsonStrs));

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
            case GpReflectContainerType::VECTOR_WRAP:
            {
                _RowToObjectPropVecWrap(dataPtr, propInfo, aRowId, colId);
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

GpReflectObject::C::Vec::SP GpDbQueryRes::RowsToObjects
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
            aProp.Value_UI8(aDataPtr) = NumOps::SConvert<u_int_8>(GetInt16(aRowId, aColId, s_int_16(0)));
        } break;
        case GpReflectType::S_INT_8:
        {
            aProp.Value_SI8(aDataPtr) = NumOps::SConvert<s_int_8>(GetInt16(aRowId, aColId, s_int_16(0)));
        } break;
        case GpReflectType::U_INT_16:
        {
            aProp.Value_UI16(aDataPtr) = NumOps::SConvert<u_int_16>(GetInt16(aRowId, aColId, s_int_16(0)));
        } break;
        case GpReflectType::S_INT_16:
        {
            aProp.Value_SI16(aDataPtr) = NumOps::SConvert<s_int_16>(GetInt16(aRowId, aColId, s_int_16(0)));
        } break;
        case GpReflectType::U_INT_32:
        {
            aProp.Value_UI32(aDataPtr) = NumOps::SConvert<u_int_32>(GetInt32(aRowId, aColId, s_int_32(0)));
        } break;
        case GpReflectType::S_INT_32:
        {
            aProp.Value_SI32(aDataPtr) = NumOps::SConvert<s_int_32>(GetInt32(aRowId, aColId, s_int_32(0)));
        } break;
        case GpReflectType::U_INT_64:
        {
            aProp.Value_UI64(aDataPtr) = NumOps::SConvert<u_int_64>(GetInt64(aRowId, aColId, s_int_64(0)));
        } break;
        case GpReflectType::S_INT_64:
        {
            aProp.Value_SI64(aDataPtr) = NumOps::SConvert<s_int_64>(GetInt64(aRowId, aColId, s_int_64(0)));
        } break;
        case GpReflectType::DOUBLE:
        {
            aProp.Value_Double(aDataPtr) = GetDouble(aRowId, aColId, 0.0);
        } break;
        case GpReflectType::FLOAT:
        {
            aProp.Value_Float(aDataPtr) = GetFloat(aRowId, aColId, 0.0f);
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
            aProp.Value_String(aDataPtr) = GetStr(aRowId, aColId, {});
        } break;
        case GpReflectType::BLOB:
        {
            aProp.Value_BLOB(aDataPtr) = GetBlob(aRowId, aColId, {}).ToByteArray();
        } break;
        case GpReflectType::OBJECT:
        {
            std::string_view    jsonStr     = GetJson(aRowId, aColId, {});
            auto&               objectVal   = aProp.Value_Object(aDataPtr);

            if (std::size(jsonStr) > 0)
            {
                GpJsonSerializer::SFromStr(jsonStr, objectVal, {});
            }
        } break;
        case GpReflectType::OBJECT_SP:
        {
            std::string_view    jsonStr     = GetJson(aRowId, aColId, {});
            auto&               objectVal   = aProp.Value_ObjectSP(aDataPtr);

            if (std::size(jsonStr) > 0)
            {
                GpReflectModel::CSP     modelSCP    = GpReflectManager::S().Find(aProp.ModelUid());
                const GpReflectModel&   model       = modelSCP.Vn();
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
            aProp.Vec_UI8(aDataPtr) = _SConvertArrayNum<u_int_8>(GetInt16Array1D(aRowId, aColId, {}));
        } break;
        case GpReflectType::S_INT_8:
        {
            aProp.Vec_SI8(aDataPtr) = _SConvertArrayNum<s_int_8>(GetInt16Array1D(aRowId, aColId, {}));
        } break;
        case GpReflectType::U_INT_16:
        {
            aProp.Vec_UI16(aDataPtr) = _SConvertArrayNum<u_int_16>(GetInt16Array1D(aRowId, aColId, {}));
        } break;
        case GpReflectType::S_INT_16:
        {
            aProp.Vec_SI16(aDataPtr) = GetInt16Array1D(aRowId, aColId, {});
        } break;
        case GpReflectType::U_INT_32:
        {
            aProp.Vec_UI32(aDataPtr) = _SConvertArrayNum<u_int_32>(GetInt32Array1D(aRowId, aColId, {}));
        } break;
        case GpReflectType::S_INT_32:
        {
            aProp.Vec_SI32(aDataPtr) = GetInt32Array1D(aRowId, aColId, {});
        } break;
        case GpReflectType::U_INT_64:
        {
            aProp.Vec_UI64(aDataPtr) = _SConvertArrayNum<u_int_64>(GetInt64Array1D(aRowId, aColId, {}));
        } break;
        case GpReflectType::S_INT_64:
        {
            aProp.Vec_SI64(aDataPtr) = GetInt64Array1D(aRowId, aColId, {});
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
            THROW_GP("Unsupported type vector of booleans"_sv); break;
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
            std::vector<GpSpanByteR> v = GetBlobArray1D(aRowId, aColId, {});
            aProp.Vec_BLOB(aDataPtr) = _SConvertArrayBytes<GpBytesArray>(v);
        } break;
        case GpReflectType::OBJECT:
        {
            THROW_GP("Unsupported type Object vector"_sv); break;
        } break;
        case GpReflectType::OBJECT_SP:
        {
            // TODO: implement
            THROW_GP_NOT_IMPLEMENTED();
            //aProp.Vec_ObjectSP(aDataPtr) = ColToObjectArray1D(aRowId, aColId, ?);
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

void    GpDbQueryRes::_RowToObjectPropVecWrap
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    aProp,
    const size_t            /*aRowId*/,
    const size_t            /*aColId*/
) const
{
    THROW_COND_GP
    (
        aProp.Type() == GpReflectType::OBJECT,
        "Unsupported type"_sv
    );

    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
    //aProp.VecWrap_Object(aDataPtr) = ColToObjectArray1D(aRowId, aColId, {});
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

}// namespace GPlatform

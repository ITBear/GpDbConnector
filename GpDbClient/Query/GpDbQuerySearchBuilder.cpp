#include "GpDbQuerySearchBuilder.hpp"
#include "GpDbQuery.hpp"

#include <GpCore2/GpUtils/Encoders/GpBase64.hpp>

namespace GPlatform {

bool    _TestChar
(
    const size_t        aCharId,
    std::string_view    aStr,
    const char          aChar
)
{
    const size_t strLength = std::size(aStr);
    if (aCharId >= strLength)
    {
        return false;
    }

    return aStr[aCharId] == aChar;
}

GpDbQuerySearchBuilder::GpDbQuerySearchBuilder (void) noexcept
{
}

GpDbQuerySearchBuilder::~GpDbQuerySearchBuilder (void) noexcept
{
}

void    GpDbQuerySearchBuilder::SEARCH_WHERE
(
    GpDbQueryBuilder&               aBuilder,
    GpReflectModel::C::Opt::CRef    aModel,
    const GpDbSearchDesc&           aSearchDesc
)
{
    ProcessFilter(aBuilder, aModel, aSearchDesc);
}

void    GpDbQuerySearchBuilder::SEARCH_ORDER
(
    GpDbQueryBuilder&       aBuilder,
    const GpDbSearchDesc&   aSearchDesc
)
{
    ProcessOrderByCond(aBuilder, aSearchDesc);
}

void    GpDbQuerySearchBuilder::SEARCH_LIMIT
(
    GpDbQueryBuilder&       aBuilder,
    const GpDbSearchDesc&   aSearchDesc
)
{
    ProcessLimitCond(aBuilder, aSearchDesc);
}

void    GpDbQuerySearchBuilder::ProcessFilter
(
    GpDbQueryBuilder&               aBuilder,
    GpReflectModel::C::Opt::CRef    aModel,
    const GpDbSearchDesc&           aSearchDesc
)
{
    std::string_view    filter          = aSearchDesc.filter;
    const size_t        filterLength    = std::size(filter);

    if (filterLength == 0)
    {
        return;
    }

    for (size_t id = 0; id < filterLength; id++)
    {
        const char ch = filter[id];

        if (ch == u8'(')
        {
            aBuilder.BRACE_BEGIN();
        } else if (ch == u8')')
        {
            aBuilder.BRACE_END();
        } else if (ch == u8'&')
        {
            aBuilder.AND();
        } else if (ch == u8'|')
        {
            aBuilder.OR();
        } else if (ch == u8'!')
        {
            if (_TestChar(id+1, filter, u8'='))
            {
                aBuilder.NOT_EQUAL();
                id++;
            } else
            {
                aBuilder.NOT();
            }
        } else if (ch == u8'=')
        {
            aBuilder.EQUAL();
        } else if (ch == u8'>')
        {
            if (_TestChar(id+1, filter, u8'='))
            {
                aBuilder.GREATER_EQUAL();
                id++;
            } else
            {
                aBuilder.GREATER();
            }
        } else if (ch == u8'<')
        {
            if (_TestChar(id+1, filter, u8'='))
            {
                aBuilder.LESS_EQUAL();
                id++;
            } else
            {
                aBuilder.LESS();
            }
        } else if (ch == u8'+')
        {
            aBuilder.ADD();
        } else if (ch == u8'-')
        {
            aBuilder.SUB();
        } else if (ch == u8'*')
        {
            aBuilder.MUL();
        } else if (ch == u8'/')
        {
            aBuilder.DIV();
        } else if (ch == u8',')
        {
            aBuilder.COMMA();
        } else if (ch == u8'@')
        {
            aBuilder.IN();
        } else if ((u8'0' <= ch) && (ch <= u8'9'))
        {
            id = ParseNum(id, filter, aBuilder);
        } else if (ch == u8'"')
        {
            id = ParseStr(id+1, filter, u8'"', u8'\\', true, aBuilder);//PROP NAME
        } else if (ch == '\'')
        {
            id = ParseStr(id+1, filter, u8'\'', u8'\\', false, aBuilder);//STRING CONSTANT
        } else if (ch == u8'L')
        {
            if (GpReflectProp::C::Opt::CRef propOpt;
                   !iLastPropName.empty()
                && aModel.has_value()
                && (propOpt = aModel.value().get().PropOpt(iLastPropName)).has_value()
                && (propOpt.value().get().FlagTest(GpReflectPropFlag::MULTILANGUAGE_STRING)))
            {
                //IN (SELECT language.find_item_ids_like('xxxxx%'))

                aBuilder.IN().BRACE_BEGIN()
                    .SELECT().RAW("language.find_item_ids_like"_sv).BRACE_BEGIN();

                id = FindAndParseStr(id+1, filter, u8'\'', u8'\\', false, aBuilder);//STRING CONSTANT

                aBuilder
                    .BRACE_END()
                .BRACE_END();
            } else
            {
                aBuilder.ILIKE();
            }
        } else if (ch == u8' ')
        {
            //Skip
        } else if (ch == u8'\n')
        {
            //Skip
        } else if (ch == u8'\r')
        {
            //Skip
        } else if (ch == u8'\t')
        {
            //Skip
        } else
        {
            THROW_GP("Parsing error: unexpected character '"_sv + ch + "' at position "_sv + id);
        }
    }
}

void    GpDbQuerySearchBuilder::ProcessOrderByCond
(
    GpDbQueryBuilder&       aBuilder,
    const GpDbSearchDesc&   aSearchDesc
)
{
    const GpDbSearchOrderDesc::C::Vec::SP& order = aSearchDesc.order;

    if (order.empty())
    {
        return;
    }

    aBuilder.ORDER_BY();

    bool isFirst = true;

    for (const GpDbSearchOrderDesc::SP& o: order)
    {
        if (!isFirst)
        {
            aBuilder.COMMA();
        } else
        {
            isFirst = false;
        }

        const GpDbSearchOrderDesc& orderDesc = o.V();
        aBuilder.COL(orderDesc.name);

        switch (orderDesc.type)
        {
            case GpDbSearchOrderType::DESC:
            {
                aBuilder.DESC();
            } break;
            case GpDbSearchOrderType::ASC:
            {
                aBuilder.ASC();
            } break;
            default:
            {
                THROW_GP("Unknown oerder type"_sv);
            }
        }
    }
}

void    GpDbQuerySearchBuilder::ProcessLimitCond
(
    GpDbQueryBuilder&       aBuilder,
    const GpDbSearchDesc&   aSearchDesc
)
{
    const size_t limit = std::min(NumOps::SConvert<size_t>(aSearchDesc.limit), size_t(250));

    if (limit == 0)
    {
        return;
    }

    aBuilder.LIMIT(limit);
}

size_t  GpDbQuerySearchBuilder::FindAndParseStr
(
    const size_t        aStartId,
    std::string_view    aStr,
    const char          aStopChar,
    const char          aEscapeChar,
    const bool          aIsPropName,
    GpDbQueryBuilder&   aBuilder
)
{
    const size_t    filterLength = std::size(aStr);
    size_t          id;

    for (id = aStartId; id < filterLength; id++)
    {
        const char ch = aStr[id];

        if (ch == '\'')
        {
            return ParseStr(id+1, aStr, aStopChar, aEscapeChar, aIsPropName, aBuilder);//STRING CONSTANT
        } else if (ch == u8' ')
        {
            //Skip
        } else if (ch == u8'\n')
        {
            //Skip
        } else if (ch == u8'\r')
        {
            //Skip
        } else if (ch == u8'\t')
        {
            //Skip
        } else
        {
            THROW_GP("Parsing error: unexpected character '"_sv + ch + "' at position "_sv + id);
        }
    }

    return id;
}

size_t  GpDbQuerySearchBuilder::ParseStr
(
    const size_t        aStartId,
    std::string_view    aStr,
    const char          aStopChar,
    const char          aEscapeChar,
    const bool          aIsPropName,
    GpDbQueryBuilder&   aBuilder
)
{
    const size_t    strLength   = std::size(aStr);
    size_t          id          = 0;
    std::string     strBuffer;

    for (id = aStartId; id < strLength; ++id)
    {
        const char ch = aStr[id];

        if (ch == aStopChar)
        {
            if (aIsPropName)//Prop name
            {
                aBuilder.COL(strBuffer);
                iLastPropName = strBuffer;
            } else//Value
            {
                const auto [valueType, newId] = DetectType(id+1, aStr);
                const bool isTypeSet = (newId > (id+1));

                id = newId;

                if      (!isTypeSet)                            aBuilder.VALUE(GpDbQueryValue(std::move(strBuffer)));
                else if (valueType == GpDbQueryValType::STRING) aBuilder.VALUE(GpDbQueryValue(std::move(strBuffer)));
                else if (valueType == GpDbQueryValType::JSON)   aBuilder.VALUE(GpDbQueryValue(GpDbQueryValueJson(std::move(strBuffer))));
                else if (valueType == GpDbQueryValType::UUID)   aBuilder.VALUE(GpDbQueryValue(GpUUID::SFromString(strBuffer)));
                else if (valueType == GpDbQueryValType::BLOB)
                {
                    aBuilder.VALUE(GpDbQueryValue(GpBase64::SDecode<GpBytesArray>(strBuffer)));
                } else if (valueType == GpDbQueryValType::INT_16) aBuilder.VALUE(GpDbQueryValue(NumOps::SConvert<s_int_16>(StrOps::SToSI64(strBuffer))));
                else if (valueType == GpDbQueryValType::INT_32) aBuilder.VALUE(GpDbQueryValue(NumOps::SConvert<s_int_32>(StrOps::SToSI64(strBuffer))));
                else if (valueType == GpDbQueryValType::INT_64) aBuilder.VALUE(GpDbQueryValue(NumOps::SConvert<s_int_64>(StrOps::SToSI64(strBuffer))));
                else if (valueType == GpDbQueryValType::FLOAT)  aBuilder.VALUE(GpDbQueryValue(float(StrOps::SToDouble(strBuffer))));
                else if (valueType == GpDbQueryValType::DOUBLE) aBuilder.VALUE(GpDbQueryValue(double(StrOps::SToDouble(strBuffer))));
                else THROW_GP("Unsupported value type '"_sv + GpDbQueryValType::SToString(valueType) + "' for text"_sv);
            }

            return id;
        } else if (ch == aEscapeChar)
        {
            if (id < (strLength-1))
            {
                const char nextCh = aStr[++id];

                if (nextCh == aStopChar)
                {
                    strBuffer += aStopChar;
                } else if (nextCh == aEscapeChar)
                {
                    strBuffer += aEscapeChar;
                } else
                {
                    THROW_GP("Parsing error: unexpected character '"_sv + nextCh + "' at position "_sv + id + ". Expected characters '"_sv + aStopChar
                     + "' or '"_sv + aEscapeChar + "' after escape character '"_sv + aEscapeChar + "'"_sv);
                }
            } else
            {
                THROW_GP("Parsing error: unexpected end of string at position "_sv + (id+1));
            }
        } else
        {
            strBuffer += ch;
        }
    }

    THROW_GP("Parsing error: unexpected end of string at position "_sv + id);
}

size_t  GpDbQuerySearchBuilder::ParseNum
(
    const size_t        aStartId,
    std::string_view    aStr,
    GpDbQueryBuilder&   aBuilder
)
{
    size_t readCount = 0;
    const std::variant<s_int_64, double> res = StrOps::SToNumeric(aStr.substr(aStartId), readCount);

    auto [valueType, newId] = DetectType(aStartId + readCount, aStr);

    const bool isTypeSet = (newId > (aStartId + readCount));

    if (std::holds_alternative<s_int_64>(res))
    {
        const s_int_64 val = std::get<s_int_64>(res);

        if (   (!isTypeSet)
            || (valueType == GpDbQueryValType::INT_64))
        {
            aBuilder.VALUE(GpDbQueryValue(val));
        } else if (valueType == GpDbQueryValType::INT_32)
        {
            aBuilder.VALUE(GpDbQueryValue(NumOps::SConvert<s_int_32>(val)));
        } else if (valueType == GpDbQueryValType::INT_16)
        {
            aBuilder.VALUE(GpDbQueryValue(NumOps::SConvert<s_int_16>(val)));
        } else if (valueType == GpDbQueryValType::FLOAT)
        {
            aBuilder.VALUE(GpDbQueryValue(float(val)));
        } else if (valueType == GpDbQueryValType::DOUBLE)
        {
            aBuilder.VALUE(GpDbQueryValue(double(val)));
        } else
        {
            THROW_GP("Parsing error: Can`t convert number to "_sv + GpDbQueryValType::SToString(valueType));
        }
    } else//double
    {
        const double val = std::get<double>(res);

        if (   (!isTypeSet)
            || (valueType == GpDbQueryValType::DOUBLE))
        {
            aBuilder.VALUE(GpDbQueryValue(val));
        } else if (valueType == GpDbQueryValType::FLOAT)
        {
            aBuilder.VALUE(GpDbQueryValue(float(val)));
        } else
        {
            THROW_GP("Parsing error: Can`t convert real number to "_sv + GpDbQueryValType::SToString(valueType));
        }
    }

    return newId;
}

std::tuple<GpDbQueryValType::EnumT, size_t> GpDbQuerySearchBuilder::DetectType
(
    const size_t        aStartId,
    std::string_view    aStr
)
{
    if (!_TestChar(aStartId, aStr, ':'))
    {
        return {GpDbQueryValType::NULL_VAL, aStartId};
    }

    const size_t            strLength   = std::size(aStr);
    GpDbQueryValType::EnumT type        = GpDbQueryValType::NULL_VAL;
    std::string_view        typeName;

    for (size_t id = aStartId + 1; id < strLength; id++)
    {
        const char ch = aStr[id];

        const bool isPermissibleChar =    ((u8'a' <= ch) || (ch >= u8'z'))
                                       || ((u8'0' <= ch) || (ch >= u8'9'));

        if (!isPermissibleChar)
        {
            typeName = std::string_view(std::data(aStr) + aStartId + 1, (id - 1) - aStartId);
            break;
        } else if (id >= (strLength-1))
        {
            typeName = std::string_view(std::data(aStr) + aStartId + 1, id - aStartId);
            break;
        }
    }

    if      (typeName == "str"_sv)  type = GpDbQueryValType::STRING;
    else if (typeName == "json"_sv) type = GpDbQueryValType::JSON;
    else if (typeName == "uuid"_sv) type = GpDbQueryValType::UUID;
    else if (typeName == "base64"_sv)   type = GpDbQueryValType::BLOB;
    else if (typeName == "i16"_sv)  type = GpDbQueryValType::INT_16;
    else if (typeName == "i32"_sv)  type = GpDbQueryValType::INT_32;
    else if (typeName == "i64"_sv)  type = GpDbQueryValType::INT_64;
    else if (typeName == "float"_sv)    type = GpDbQueryValType::FLOAT;
    else if (typeName == "double"_sv)   type = GpDbQueryValType::DOUBLE;
    else THROW_GP("Parsing error: unknown type '"_sv + typeName + "' at position "_sv + (aStartId + 1));

    return {type, aStartId + std::size(typeName)};
}

}// namespace GPlatform

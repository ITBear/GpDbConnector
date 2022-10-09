#include "GpDbQuerySearchBuilder.hpp"
#include "GpDbQuery.hpp"

namespace GPlatform {

GpDbQuerySearchBuilder::GpDbQuerySearchBuilder (void) noexcept
{
}

GpDbQuerySearchBuilder::~GpDbQuerySearchBuilder (void) noexcept
{
}

void    GpDbQuerySearchBuilder::SEARCH_WHERE
(
    GpDbQueryBuilder&       aBuilder,
    const GpDbSearchDesc&   aSearchDesc
)
{
    ProcessFilter(aBuilder, aSearchDesc);
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
    GpDbQueryBuilder&       aBuilder,
    const GpDbSearchDesc&   aSearchDesc
)
{
    std::string_view    filter          = aSearchDesc.filter;
    const size_t        filterLength    = filter.length();

    if (filterLength == 0)
    {
        return;
    }

    for (size_t id = 0; id < filterLength; id++)
    {
        const char ch = filter[id];

        if (ch == '(')
        {
            aBuilder.BRACE_BEGIN();
        } else if (ch == ')')
        {
            aBuilder.BRACE_END();
        } else if (ch == '&')
        {
            aBuilder.AND();
        } else if (ch == '|')
        {
            aBuilder.OR();
        } else if (ch == '!')
        {
            if (TestChar(id+1, filter, '='))
            {
                aBuilder.NOT_EQUAL();
                id++;
            } else
            {
                aBuilder.NOT();
            }
        } else if (ch == '=')
        {
            aBuilder.EQUAL();
        } else if (ch == '>')
        {
            if (TestChar(id+1, filter, '='))
            {
                aBuilder.GREATER_EQUAL();
                id++;
            } else
            {
                aBuilder.GREATER();
            }
        } else if (ch == '<')
        {
            if (TestChar(id+1, filter, '='))
            {
                aBuilder.LESS_EQUAL();
                id++;
            } else
            {
                aBuilder.LESS();
            }
        } else if (ch == '+')
        {
            aBuilder.ADD();
        } else if (ch == '-')
        {
            aBuilder.SUB();
        } else if (ch == '*')
        {
            aBuilder.MUL();
        } else if (ch == '/')
        {
            aBuilder.DIV();
        } else if (ch == ',')
        {
            aBuilder.COMMA();
        } else if (ch == '@')
        {
            aBuilder.IN();
        } else if (('0' <= ch) && (ch <= '9'))
        {
            id = ParseNum(id, filter, aBuilder);
        } else if (ch == '"')
        {
            id = ParseStr(id+1, filter, '"', '\\', true, aBuilder);
        } else if (ch == '\'')
        {
            id = ParseStr(id+1, filter, '\'', '\\', false, aBuilder);
        } else if (ch == ' ')
        {
            //Skip
        } else if (ch == '\n')
        {
            //Skip
        } else if (ch == '\r')
        {
            //Skip
        } else if (ch == '\t')
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

    if (order.size() == 0)
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

        switch (orderDesc.type.Value())
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
    const size_t limit = std::min(aSearchDesc.limit, size_t(250));

    if (limit == 0)
    {
        return;
    }

    aBuilder.LIMIT(limit);
}

bool    GpDbQuerySearchBuilder::TestChar
(
    const size_t        aCharId,
    std::string_view    aStr,
    const char          aChar
) const
{
    const size_t strLength = aStr.size();
    if (aCharId >= strLength)
    {
        return false;
    }

    return aStr[aCharId] == aChar;
}

size_t  GpDbQuerySearchBuilder::ParseStr
(
    const size_t        aStartId,
    std::string_view    aStr,
    const char          aStopChar,
    const char          aEscapeChar,
    const bool          aIsName,
    GpDbQueryBuilder&   aBuilder
)
{
    const size_t    strLength   = aStr.size();
    size_t          id          = 0;
    std::string     strBuffer;

    for (id = aStartId; id < strLength; ++id)
    {
        const char ch = aStr[id];

        if (ch == aStopChar)
        {
            if (aIsName)
            {
                aBuilder.COL(strBuffer);
            } else
            {
                auto [userType, newId] = DetectType(id+1, aStr, GpDbQueryValType::STRING);
                id = newId;

                if (userType == GpDbQueryValType::STRING)
                {
                    aBuilder.VALUE(userType, GpDbQueryValue(std::move(strBuffer)));
                } else if (userType == GpDbQueryValType::JSON)
                {
                    aBuilder.VALUE(userType, GpDbQueryValue(std::move(strBuffer)));
                } else if (userType == GpDbQueryValType::UUID)
                {
                    aBuilder.VALUE(userType, GpDbQueryValue(GpUUID::SFromString(strBuffer)));
                } else if (userType == GpDbQueryValType::BLOB)
                {
                    aBuilder.VALUE(userType, GpDbQueryValue(GpBase64::SDecodeToByteArray(strBuffer)));
                } else
                {
                    THROW_GP("Parsing error: Can`t convert string to "_sv + GpDbQueryValType::SToString(userType));
                }
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

    auto [userType, newId] = DetectType(aStartId + readCount, aStr, GpDbQueryValType::NULL_VAL);

    if (std::holds_alternative<s_int_64>(res))
    {
        if (   (userType == GpDbQueryValType::NULL_VAL)
            || (userType == GpDbQueryValType::INT_64))
        {
            aBuilder.VALUE(GpDbQueryValType::INT_64, GpDbQueryValue(std::get<s_int_64>(res)));
        } else if (userType == GpDbQueryValType::INT_32)
        {
            aBuilder.VALUE(GpDbQueryValType::INT_32, GpDbQueryValue(NumOps::SConvert<s_int_32>(std::get<s_int_64>(res))));
        } else if (userType == GpDbQueryValType::INT_16)
        {
            aBuilder.VALUE(GpDbQueryValType::INT_16, GpDbQueryValue(NumOps::SConvert<s_int_16>(std::get<s_int_64>(res))));
        } else if (userType == GpDbQueryValType::FLOAT)
        {
            aBuilder.VALUE(GpDbQueryValType::FLOAT, GpDbQueryValue(float(std::get<s_int_64>(res))));
        } else if (userType == GpDbQueryValType::DOUBLE)
        {
            aBuilder.VALUE(GpDbQueryValType::DOUBLE, GpDbQueryValue(double(std::get<s_int_64>(res))));
        } else
        {
            THROW_GP("Parsing error: Can`t convert number to "_sv + GpDbQueryValType::SToString(userType));
        }
    } else//double
    {
        if (   (userType == GpDbQueryValType::NULL_VAL)
            || (userType == GpDbQueryValType::DOUBLE))
        {
            aBuilder.VALUE(GpDbQueryValType::DOUBLE, GpDbQueryValue(std::get<double>(res)));
        } else if (userType == GpDbQueryValType::FLOAT)
        {
            aBuilder.VALUE(GpDbQueryValType::FLOAT, GpDbQueryValue(float(std::get<double>(res))));
        } else
        {
            THROW_GP("Parsing error: Can`t convert real number to "_sv + GpDbQueryValType::SToString(userType));
        }
    }

    return newId;
}

std::tuple<GpDbQueryValType::EnumT, size_t> GpDbQuerySearchBuilder::DetectType
(
    const size_t                    aStartId,
    std::string_view                aStr,
    const GpDbQueryValType::EnumT   aDefaultType
)
{
    if (!TestChar(aStartId, aStr, ':'))
    {
        return {aDefaultType, aStartId-1};
    }

    const size_t            strLength   = aStr.size();
    GpDbQueryValType::EnumT type        = aDefaultType;
    std::string_view        typeName;

    for (size_t id = aStartId + 1; id < strLength; id++)
    {
        const char ch = aStr[id];

        if ((ch < 'a') || ('z' < ch))
        {
            typeName = std::string_view(aStr.data() + aStartId + 1, (id - 1) - aStartId);
            break;
        } else if (id >= (strLength-1))
        {
            typeName = std::string_view(aStr.data() + aStartId + 1, id - aStartId);
            break;
        }
    }

    if (typeName == "str"_sv)
    {
        type = GpDbQueryValType::STRING;
    } else if (typeName == "json"_sv)
    {
        type = GpDbQueryValType::JSON;
    } else if (typeName == "uuid"_sv)
    {
        type = GpDbQueryValType::UUID;
    } else if (typeName == "base64"_sv)
    {
        type = GpDbQueryValType::UUID;
    } else if (typeName == "i16"_sv)
    {
        type = GpDbQueryValType::INT_16;
    } else if (typeName == "i32"_sv)
    {
        type = GpDbQueryValType::INT_32;
    } else if (typeName == "i64"_sv)
    {
        type = GpDbQueryValType::INT_64;
    } else if (typeName == "float"_sv)
    {
        type = GpDbQueryValType::FLOAT;
    } else if (typeName == "double"_sv)
    {
        type = GpDbQueryValType::DOUBLE;
    } else
    {
        THROW_GP("Parsing error: unknown type '"_sv + typeName + "' at position "_sv + (aStartId + 1));
    }

    return {type, aStartId + typeName.length()};
}

}//namespace GPlatform

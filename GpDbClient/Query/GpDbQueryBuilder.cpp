#include "GpDbQueryBuilder.hpp"
#include "GpDbQuerySearchBuilder.hpp"
#include "../GpDbException.hpp"

namespace GPlatform {

const GpDbQueryBuilder::BindStrsT   GpDbQueryBuilder::sBindStrs
{
    "int2",     //INT_16,
    "int2[]",   //INT_16_ARRAY_1D,
    "int4",     //INT_32,
    "int4[]",   //INT_32_ARRAY_1D,
    "int8",     //INT_64,
    "int8[]",   //INT_64_ARRAY_1D,
    "float8",   //DOUBLE,
    "float8[]", //DOUBLE_ARRAY_1D,
    "float4",   //FLOAT,
    "float4[]", //FLOAT_ARRAY_1D,
    "text",     //STRING,
    "text[]",   //STRING_ARRAY_1D,
    "jsonb",    //JSON,
    "jsonb[]",  //JSON_ARRAY_1D,
    "uuid",     //UUID,
    "uuid[]",   //UUID_ARRAY_1D,
    "bytea",    //BLOB,
    "bytea[]",  //BLOB_ARRAY_1D,
    "boolean",  //BOOLEAN,
    "boolean[]",//BOOLEAN_ARRAY_1D,
    "",         //NULL_VAL
};

const GpDbQueryBuilder::BindCacheT GpDbQueryBuilder::sTypeBind =
{
    std::tuple<std::string, GpDbQueryValType::EnumT>
    {"",            GpDbQueryValType::NULL_VAL},        //NOT_SET
    {"::int2",      GpDbQueryValType::INT_16},          //U_INT_8,
    {"::int2",      GpDbQueryValType::INT_16},          //S_INT_8,
    {"::int2",      GpDbQueryValType::INT_16},          //U_INT_16,
    {"::int2",      GpDbQueryValType::INT_16},          //S_INT_16,
    {"::int4",      GpDbQueryValType::INT_32},          //U_INT_32,
    {"::int4",      GpDbQueryValType::INT_32},          //S_INT_32,
    {"::int8",      GpDbQueryValType::INT_64},          //U_INT_64,
    {"::int8",      GpDbQueryValType::INT_64},          //S_INT_64,
    {"::float8",    GpDbQueryValType::DOUBLE},          //DOUBLE,
    {"::float4",    GpDbQueryValType::FLOAT},           //FLOAT,
    {"::boolean",   GpDbQueryValType::BOOLEAN},         //BOOLEAN,
    {"::uuid",      GpDbQueryValType::UUID},            //UUID,
    {"::text",      GpDbQueryValType::STRING},          //STRING,
    {"::bytea",     GpDbQueryValType::BLOB},            //BLOB,
    {"::jsonb",     GpDbQueryValType::JSON},            //OBJECT,
    {"::jsonb",     GpDbQueryValType::JSON},            //OBJECT_SP,
    {"",            GpDbQueryValType::STRING},          //ENUM,
    {"",            GpDbQueryValType::STRING_ARRAY_1D}  //ENUM_FLAGS TODO: ? can be any ENUM DB type (CREATE TYPE schema.type_name AS ENUM ('A', 'B', 'C');
                                                        // then valueBind must be = ::schema.type_name[]
};

const GpDbQueryBuilder::BindCacheT  GpDbQueryBuilder::sTypeBindVec =
{
    std::tuple<std::string, GpDbQueryValType::EnumT>
    {"",            GpDbQueryValType::NULL_VAL},        //NOT_SET
    {"::int2[]",    GpDbQueryValType::INT_16_ARRAY_1D}, //U_INT_8,
    {"::int2[]",    GpDbQueryValType::INT_16_ARRAY_1D}, //S_INT_8,
    {"::int2[]",    GpDbQueryValType::INT_16_ARRAY_1D}, //U_INT_16,
    {"::int2[]",    GpDbQueryValType::INT_16_ARRAY_1D}, //S_INT_16,
    {"::int4[]",    GpDbQueryValType::INT_32_ARRAY_1D}, //U_INT_32,
    {"::int4[]",    GpDbQueryValType::INT_32_ARRAY_1D}, //S_INT_32,
    {"::int8[]",    GpDbQueryValType::INT_64_ARRAY_1D}, //U_INT_64,
    {"::int8[]",    GpDbQueryValType::INT_64_ARRAY_1D}, //S_INT_64,
    {"::float8[]",  GpDbQueryValType::DOUBLE_ARRAY_1D}, //DOUBLE,
    {"::float4[]",  GpDbQueryValType::FLOAT_ARRAY_1D},  //FLOAT,
    {"::boolean[]", GpDbQueryValType::BOOLEAN_ARRAY_1D},//BOOLEAN,
    {"::uuid[]",    GpDbQueryValType::UUID_ARRAY_1D},   //UUID,
    {"::text[]",    GpDbQueryValType::STRING_ARRAY_1D}, //STRING,
    {"::bytea[]",   GpDbQueryValType::BLOB_ARRAY_1D},   //BLOB,
    {"",            GpDbQueryValType::NULL_VAL},        //OBJECT,
    {"::jsonb[]",   GpDbQueryValType::JSON_ARRAY_1D},   //OBJECT_SP,
    {"",            GpDbQueryValType::NULL_VAL},        //ENUM,
    {"",            GpDbQueryValType::NULL_VAL}         //ENUM_FLAGS
};

const GpDbQueryBuilder::BindCacheT  GpDbQueryBuilder::sTypeBindMap =
{
};

GpDbQueryBuilder::GpDbQueryBuilder (void) noexcept
{
}

GpDbQueryBuilder::~GpDbQueryBuilder (void) noexcept
{
}

GpDbQueryBuilder&   GpDbQueryBuilder::RAW (std::string_view aRawStr)
{
    iQueryStr
        .append(aRawStr);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BRACE_BEGIN (void)
{
    iQueryStr
        .append("("_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BRACE_END (void)
{
    iQueryStr
        .append(")"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COMMA (void)
{
    iQueryStr
        .append(","_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::AND (void)
{
    _CheckForSpace();

    iQueryStr
        .append("AND"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::OR (void)
{
    _CheckForSpace();

    iQueryStr
        .append("OR"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::NOT (void)
{
    _CheckForSpace();

    iQueryStr
        .append("NOT"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BITWISE_AND (void)
{
    iQueryStr
        .append("&"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BITWISE_OR (void)
{
    iQueryStr
        .append("|"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::EQUAL (void)
{
    iQueryStr
        .append("="_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::NOT_EQUAL (void)
{
    iQueryStr
        .append("!="_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::GREATER (void)
{
    iQueryStr
        .append(">"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::GREATER_EQUAL (void)
{
    iQueryStr
        .append(">="_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LESS (void)
{
    iQueryStr
        .append("<"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LESS_EQUAL (void)
{
    iQueryStr
        .append("<="_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ASSIGN (void)
{
    iQueryStr
        .append("="_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ADD (void)
{
    iQueryStr
        .append("+"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SUB (void)
{
    iQueryStr
        .append("-"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::MUL (void)
{
    iQueryStr
        .append("*"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::DIV (void)
{
    iQueryStr
        .append("/"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COUNT_1 (void)
{
    _CheckForSpace();

    iQueryStr
        .append("COUNT(1)"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COALESCE_BEGIN (void)
{
    _CheckForSpace();

    iQueryStr
        .append("COALESCE("_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COALESCE_END (void)
{
    iQueryStr
        .append(")"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ASC (void)
{
    _CheckForSpace();

    iQueryStr
        .append("ASC"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::DESC (void)
{
    _CheckForSpace();

    iQueryStr
        .append("DESC"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::NULLS (void)
{
    _CheckForSpace();

    iQueryStr
        .append("NULLS"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LAST (void)
{
    _CheckForSpace();

    iQueryStr
        .append("LAST"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ON (void)
{
    _CheckForSpace();

    iQueryStr
        .append("ON"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::AS (void)
{
    _CheckForSpace();

    iQueryStr
        .append("AS"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::AS (std::string_view aName)
{
    _CheckForSpace();

    iQueryStr
        .append("AS "_sv);

    _AppendName(aName);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::IN (void)
{
    _CheckForSpace();

    iQueryStr
        .append("IN"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ANY
(
    const GpDbQueryValType::EnumT   aValueType,
    std::string_view                aTypeCast
)
{
    _CheckForSpace();

    iQueryStr.append("ANY("_sv);
    VALUE(aValueType);
    iQueryStr.append("::"_sv).append(aTypeCast).append(")"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BETWEEN (void)
{
    _CheckForSpace();

    iQueryStr
        .append("BETWEEN"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BETWEEN
(
    const GpDbQueryValType::EnumT aValueTypeA,
    const GpDbQueryValType::EnumT aValueTypeB
)
{
    BETWEEN().VALUE(aValueTypeA).AND().VALUE(aValueTypeB);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LIKE (const GpDbQueryValType::EnumT aValueType)
{
    _CheckForSpace();

    iQueryStr
        .append("LIKE "_sv)
        .append(_ValueBind(aValueType));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::IS_NULL (void)
{
    _CheckForSpace();

    iQueryStr
        .append("IS NULL"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::IS_NOT_NULL (void)
{
    _CheckForSpace();

    iQueryStr
        .append("IS NOT NULL"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUES_BEGIN (void)
{
    _CheckForSpace();

    iQueryStr
        .append("VALUES("_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUES_END (void)
{
    iQueryStr
        .append(")"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUES (void)
{
    _CheckForSpace();

    iQueryStr
        .append("VALUES"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::WITH (std::string_view aName)
{
    _CheckForSpace();

    iQueryStr
        .append("WITH "_sv);

    _AppendName(aName);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::INSERT_INTO
(
    std::string_view aSchema,
    std::string_view aTable
)
{
    _CheckForSpace();

    iQueryStr
        .append("INSERT INTO "_sv);

    _AppendName(aSchema);

    iQueryStr
        .append("."_sv);

    _AppendName(aTable);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SELECT (void)
{
    _CheckForSpace();

    iQueryStr
        .append("SELECT"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SELECT_ALL (void)
{
    _CheckForSpace();

    iQueryStr
        .append("SELECT *"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::FROM
(
    std::string_view aSchema,
    std::string_view aTable
)
{
    _CheckForSpace();

    iQueryStr
        .append("FROM "_sv);

    _AppendName(aSchema);

    iQueryStr
        .append("."_sv);

    _AppendName(aTable);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::FROM (void)
{
    _CheckForSpace();

    iQueryStr
        .append("FROM"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::WHERE (void)
{
    _CheckForSpace();

    iQueryStr
        .append("WHERE"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::UPDATE
(
    std::string_view aSchema,
    std::string_view aTable
)
{
    _CheckForSpace();

    iQueryStr
        .append("UPDATE "_sv);

    _AppendName(aSchema);

    iQueryStr
        .append("."_sv);

    _AppendName(aTable);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SET (void)
{
    _CheckForSpace();

    iQueryStr
        .append("SET"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ORDER_BY (void)
{
    _CheckForSpace();

    iQueryStr
        .append("ORDER BY"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LIMIT (const size_t aValue)
{
    _CheckForSpace();

    iQueryStr
        .append("LIMIT "_sv)
        .append(StrOps::SToString(aValue));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LIMIT (const GpDbQueryValType::EnumT aValueType)
{
    _CheckForSpace();

    iQueryStr
        .append("LIMIT "_sv);

    VALUE(aValueType);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::OFFSET (const size_t aValue)
{
    _CheckForSpace();

    iQueryStr
        .append("OFFSET "_sv)
        .append(StrOps::SToString(aValue));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::OFFSET (const GpDbQueryValType::EnumT aValueType)
{
    _CheckForSpace();

    iQueryStr
        .append("OFFSET "_sv);

    VALUE(aValueType);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::RETURNING (void)
{
    _CheckForSpace();

    iQueryStr
        .append("RETURNING"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::RETURNING_ALL (void)
{
    _CheckForSpace();

    iQueryStr
        .append("RETURNING *"_sv);

    return *this;
}

/*GpDbQueryBuilder& GpDbQueryBuilder::RETURNING (std::string_view aRaw)
{
    CheckForSpace();

    iQueryStr
        .append("RETURNING"_sv)
        .append(" "_sv);

    RAW(aRaw);

    return *this;
}*/

GpDbQueryBuilder&   GpDbQueryBuilder::FOR_UPDATE (void)
{
    _CheckForSpace();

    iQueryStr
        .append("FOR UPDATE"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SKIP_LOCKED (void)
{
    _CheckForSpace();

    iQueryStr
        .append("SKIP LOCKED"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::DELETE_FROM
(
    std::string_view aSchema,
    std::string_view aTable
)
{
    _CheckForSpace();

    iQueryStr
        .append("DELETE FROM "_sv);

    _AppendName(aSchema);

    iQueryStr
        .append("."_sv);

    _AppendName(aTable);

    return *this;

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ON_CONFLICT (void)
{
    _CheckForSpace();

    iQueryStr
        .append("ON CONFLICT"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::DO_NOTHING (void)
{
    _CheckForSpace();

    iQueryStr
        .append("DO NOTHING"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::NOT_EXISTS (void)
{
    _CheckForSpace();

    iQueryStr
        .append("NOT EXISTS"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::INNER_JOIN
(
    std::string_view aSchema,
    std::string_view aTable
)
{
    _CheckForSpace();

    iQueryStr
        .append("INNER JOIN "_sv);

    _AppendName(aSchema);

    iQueryStr
        .append("."_sv);

    _AppendName(aTable);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LEFT_JOIN
(
    std::string_view aSchema,
    std::string_view aTable
)
{
    _CheckForSpace();

    iQueryStr
        .append("LEFT JOIN "_sv);

    _AppendName(aSchema);

    iQueryStr
        .append("."_sv);

    _AppendName(aTable);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::RIGHT_JOIN
(
    std::string_view aSchema,
    std::string_view aTable
)
{
    _CheckForSpace();

    iQueryStr
        .append("RIGHT JOIN "_sv);

    _AppendName(aSchema);

    iQueryStr
        .append("."_sv);

    _AppendName(aTable);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COL (std::string_view aName)
{
    _CheckForSpace();
    _AppendName(aName);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COL_EQUAL_TO
(
    std::string_view                aName,
    const GpDbQueryValType::EnumT   aValueType
)
{
    return COL(aName).EQUAL().VALUE(aValueType);
}

GpDbQueryBuilder&   GpDbQueryBuilder::COL_IS_NOT_NUUL (std::string_view aName)
{
    return COL(aName).IS_NOT_NULL();
}

GpDbQueryBuilder&   GpDbQueryBuilder::COL_IS_NUUL (std::string_view aName)
{
    return COL(aName).IS_NULL();
}

GpDbQueryBuilder&   GpDbQueryBuilder::COLS (const std::vector<std::string_view>& aNames)
{
    _CheckForSpace();

    auto getFn = [&](const auto& i) -> std::string
    {
        std::vector<std::string_view>   parts = StrOps::SSplit(*i, '.', 0, 0, Algo::SplitMode::COUNT_ZERO_LENGTH_PARTS);
        std::string                 s;

        for (const auto& p: parts)
        {
            if (s.length() > 0)
            {
                s.append("."_sv);
            }

            s.append("\""_sv).append(_SCheckIfName(p)).append("\""_sv);
        }

        return s;
    };

    iQueryStr
        .append(StrOps::SJoin<std::string>(aNames, getFn, ","_sv))
        .append(" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COL_ASSIGN
(
    std::string_view                aName,
    const GpDbQueryValType::EnumT   aValueType
)
{
    COL(aName).EQUAL().VALUE(aValueType);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE_NULL (void)
{
    _CheckForSpace();

    iQueryStr
        .append("NULL"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE (const GpDbQueryValType::EnumT aValueType)
{
    _CheckForSpace();

    iQueryStr
        .append(_ValueBind(aValueType));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE
(
    const GpDbQueryValType::EnumT   aValueType,
    GpDbQueryValue&&                aValue
)
{
    _CheckForSpace();

    iQueryStr
        .append(_ValueBind(aValueType));

    iValues.emplace_back(std::move(aValue));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE
(
    const GpDbQueryValType::EnumT   aValueType,
    std::string_view                aTypeCast
)
{
    _CheckForSpace();

    iQueryStr
        .append(_ValueBind(aValueType, aTypeCast));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE (std::string_view aValue)
{
    _CheckForSpace();

    iQueryStr
        .append("'"_sv)
        .append(_SEscape(aValue))
        .append("'"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE
(
    std::string_view    aValue,
    std::string_view    aTypeCast
)
{
    VALUE(aValue);

    if (aTypeCast.length() > 0)
    {
        iQueryStr
            .append("::"_sv).append(aTypeCast);
    }

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE (const s_int_64 aValue)
{
    _CheckForSpace();

    iQueryStr
        .append(StrOps::SFromSI64(aValue));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE (const GpBool aValue)
{
    _CheckForSpace();

    iQueryStr
        .append(aValue.Value() ? "true"_sv : "false"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE
(
    const GpEnum&       aValue,
    std::string_view    aTypeCast
)
{
    return VALUE(aValue.ToString(), aTypeCast);
}

GpDbQueryBuilder&   GpDbQueryBuilder::OBJECT_NAMES
(
    const GpReflectModel&               aModel,
    const GpDbQueryBuilderMode::EnumT   aMode
)
{
    _CheckForSpace();

    const auto info = _SFromModel(aModel, aMode);

    auto getFn = [&](auto& i) -> std::string
    {
        return "\""_sv + _SCheckIfName(i->name) + "\""_sv;
    };

    iQueryStr
        .append(StrOps::SJoin<std::string>(info, getFn, ","_sv));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::OBJECT_BINDS
(
    const GpReflectModel&               aModel,
    const GpDbQueryBuilderMode::EnumT   aMode
)
{
    _CheckForSpace();

    const auto info = _SFromModel(aModel, aMode);

    auto getFn = [&](auto& i) -> std::string
    {
        iTypes.emplace_back(i->type);

        std::string res;
        res.reserve(10);

        res
            .append("$"_sv)
            .append(StrOps::SToString<size_t>(iTypes.size()))
            .append(i->bindType);

        return res;
    };

    iQueryStr
        .append(StrOps::SJoin<std::string>(info, getFn, ","_sv));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::OBJECT_FOR_UPDATE
(
    const GpReflectModel&               aModel,
    const GpDbQueryBuilderMode::EnumT   aMode
)
{
    _CheckForSpace();

    const auto info = _SFromModel(aModel, aMode);

    auto getFn = [&](auto& i) -> std::string
    {
        iTypes.emplace_back(i->type);

        std::string res = "\""_sv + _SCheckIfName(i->name) + "\"="_sv;

        res
            .append("$"_sv)
            .append(StrOps::SToString<size_t>(iTypes.size()))
            .append(i->bindType);

        return res;
    };

    iQueryStr
        .append(StrOps::SJoin<std::string>(info, getFn, ","_sv));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SEARCH_WHERE (const GpDbSearchDesc& aSearchDesc)
{
    if (iQuerySearchBuilder.IsNULL())
    {
        iQuerySearchBuilder = MakeSP<GpDbQuerySearchBuilder>();
    }

    iQuerySearchBuilder.V().SEARCH_WHERE(*this, aSearchDesc);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SEARCH_ORDER (const GpDbSearchDesc& aSearchDesc)
{
    if (iQuerySearchBuilder.IsNULL())
    {
        iQuerySearchBuilder = MakeSP<GpDbQuerySearchBuilder>();
    }

    iQuerySearchBuilder.V().SEARCH_ORDER(*this, aSearchDesc);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SEARCH_LIMIT (const GpDbSearchDesc& aSearchDesc)
{
    if (iQuerySearchBuilder.IsNULL())
    {
        iQuerySearchBuilder = MakeSP<GpDbQuerySearchBuilder>();
    }

    iQuerySearchBuilder.V().SEARCH_LIMIT(*this, aSearchDesc);

    return *this;
}

std::string GpDbQueryBuilder::_ValueBind (const GpDbQueryValType::EnumT aValueType)
{
    return _ValueBind(aValueType, sBindStrs.at(size_t(aValueType)));
}

std::string GpDbQueryBuilder::_ValueBind
(
    const GpDbQueryValType::EnumT   aValueType,
    std::string_view                aTypeCast
)
{
    iTypes.emplace_back(aValueType);

    std::string res;

    res
        .append("$"_sv)
        .append(StrOps::SToString<size_t>(iTypes.size()));

    if (aTypeCast.length() > 0)
    {
        res.append("::"_sv).append(aTypeCast);
    }

    return res;
}

void    GpDbQueryBuilder::_CheckForSpace (void)
{
    if (iQueryStr.length() > 0)
    {
        const char ch = iQueryStr.at(iQueryStr.length() - 1);

        if (   (ch != ' ')
            && (ch != ')')
            && (ch != '('))
        {
            iQueryStr.append(" "_sv);
        }
    } else
    {
        iQueryStr.reserve(512);
    }
}

void    GpDbQueryBuilder::_AppendName (std::string_view aName)
{
    iQueryStr
        .append("\""_sv)
        .append(_SCheckIfName(aName))
        .append("\""_sv);

    std::string s;
}

std::string_view    GpDbQueryBuilder::_SCheckIfName (std::string_view aStr)
{
    THROW_COND_DB
    (
        aStr.length() <= 255,
        GpDbExceptionCode::QUERY_ERROR,
        "Name length must be <= 255"_sv
    );

    for (const char ch: aStr)
    {
        THROW_COND_DB
        (
            ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')) || (ch == '_'),
            GpDbExceptionCode::QUERY_ERROR,
            [&](){return "Name '"_sv + aStr + "' contains wrong character '"_sv + ch + "'"_sv;}
        );
    }

    return aStr;
}

std::vector<GpDbQueryBuilder::TypeInfo> GpDbQueryBuilder::_SFromModel
(
    const GpReflectModel&               aModel,
    const GpDbQueryBuilderMode::EnumT   aMode
)
{
    std::vector<TypeInfo> res;

    const auto&     props       = aModel.Props();
    const size_t    propsCout   = props.size();

    res.reserve(propsCout);

    for (const GpReflectProp& propInfo: aModel.Props())
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

        const GpReflectContainerType::EnumT propContainer = propInfo.Container();

        std::string             valueBind;
        GpDbQueryValType::EnumT valueType;

        switch (propContainer)
        {
            case GpReflectContainerType::NO:
            {
                std::tie(valueBind, valueType) = sTypeBind.at(int(propInfo.Type()));
            } break;
            case GpReflectContainerType::VECTOR:
            {
                std::tie(valueBind, valueType) = sTypeBindVec.at(int(propInfo.Type()));
            } break;
            case GpReflectContainerType::MAP:
            {
                //TODO: implement
                THROW_GP_NOT_IMPLEMENTED();
                //std::tie(valueBind, valueType) = sTypeBindMap.at(int(propInfo.Type()));
            } break;
            default:
            {
                THROW_GP("Unknown container type "_sv + GpReflectContainerType::SToString(propContainer));
            }
        }

        res.emplace_back(std::string(propInfo.Name()), std::move(valueBind), std::move(valueType));
    }

    return res;
}

std::string GpDbQueryBuilder::_SEscape (std::string_view aStr)
{
    //Check if need to escape
    {
        bool isNeedToEscape = false;

        for (const char ch: aStr)
        {
            if (ch == '\'')
            {
                isNeedToEscape = true;
                break;
            }
        }

        if (!isNeedToEscape)
        {
            return std::string(aStr);
        }
    }

    //Do escape
    std::string escapedStr;
    escapedStr.reserve(NumOps::SMul(aStr.size(), size_t(2)));

    for (const char ch: aStr)
    {
        if (ch != '\'')
        {
            escapedStr.append(ch, 1);
        } else
        {
            escapedStr.append("''");
        }
    }

    return escapedStr;
}

}//GPlatform

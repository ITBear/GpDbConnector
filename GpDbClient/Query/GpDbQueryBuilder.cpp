#include "GpDbQueryBuilder.hpp"
#include "GpDbQuerySearchBuilder.hpp"
#include "../GpDbException.hpp"

namespace GPlatform {

const GpDbQueryBuilder::BindStrsT   GpDbQueryBuilder::sBindStrs
{
    u8"int2",       //INT_16,
    u8"int2[]",     //INT_16_ARRAY_1D,
    u8"int4",       //INT_32,
    u8"int4[]",     //INT_32_ARRAY_1D,
    u8"int8",       //INT_64,
    u8"int8[]",     //INT_64_ARRAY_1D,
    u8"float8",     //DOUBLE,
    u8"float8[]",   //DOUBLE_ARRAY_1D,
    u8"float4",     //FLOAT,
    u8"float4[]",   //FLOAT_ARRAY_1D,
    u8"text",       //STRING,
    u8"text[]",     //STRING_ARRAY_1D,
    u8"jsonb",      //JSON,
    u8"jsonb[]",    //JSON_ARRAY_1D,
    u8"uuid",       //UUID,
    u8"uuid[]",     //UUID_ARRAY_1D,
    u8"bytea",      //BLOB,
    u8"bytea[]",    //BLOB_ARRAY_1D,
    u8"boolean",    //BOOLEAN,
    u8"boolean[]",  //BOOLEAN_ARRAY_1D,
    u8"",           //NULL_VAL
};

const GpDbQueryBuilder::BindCacheT GpDbQueryBuilder::sTypeBind =
{
    std::tuple<std::u8string, GpDbQueryValType::EnumT>
    {u8"",          GpDbQueryValType::NULL_VAL},        //NOT_SET
    {u8"::int2",    GpDbQueryValType::INT_16},          //U_INT_8,
    {u8"::int2",    GpDbQueryValType::INT_16},          //S_INT_8,
    {u8"::int2",    GpDbQueryValType::INT_16},          //U_INT_16,
    {u8"::int2",    GpDbQueryValType::INT_16},          //S_INT_16,
    {u8"::int4",    GpDbQueryValType::INT_32},          //U_INT_32,
    {u8"::int4",    GpDbQueryValType::INT_32},          //S_INT_32,
    {u8"::int8",    GpDbQueryValType::INT_64},          //U_INT_64,
    {u8"::int8",    GpDbQueryValType::INT_64},          //S_INT_64,
    {u8"::float8",  GpDbQueryValType::DOUBLE},          //DOUBLE,
    {u8"::float4",  GpDbQueryValType::FLOAT},           //FLOAT,
    {u8"::boolean", GpDbQueryValType::BOOLEAN},         //BOOLEAN,
    {u8"::uuid",    GpDbQueryValType::UUID},            //UUID,
    {u8"::text",    GpDbQueryValType::STRING},          //STRING,
    {u8"::bytea",   GpDbQueryValType::BLOB},            //BLOB,
    {u8"::jsonb",   GpDbQueryValType::JSON},            //OBJECT,
    {u8"::jsonb",   GpDbQueryValType::JSON},            //OBJECT_SP,
    {u8"",          GpDbQueryValType::STRING},          //ENUM,
    {u8"",          GpDbQueryValType::STRING_ARRAY_1D}  //ENUM_FLAGS TODO: ? can be any ENUM DB type (CREATE TYPE schema.type_name AS ENUM ('A', 'B', 'C');
                                                        // then valueBind must be = ::schema.type_name[]
};

const GpDbQueryBuilder::BindCacheT  GpDbQueryBuilder::sTypeBindVec =
{
    std::tuple<std::u8string, GpDbQueryValType::EnumT>
    {u8"",          GpDbQueryValType::NULL_VAL},        //NOT_SET
    {u8"::int2[]",  GpDbQueryValType::INT_16_ARRAY_1D}, //U_INT_8,
    {u8"::int2[]",  GpDbQueryValType::INT_16_ARRAY_1D}, //S_INT_8,
    {u8"::int2[]",  GpDbQueryValType::INT_16_ARRAY_1D}, //U_INT_16,
    {u8"::int2[]",  GpDbQueryValType::INT_16_ARRAY_1D}, //S_INT_16,
    {u8"::int4[]",  GpDbQueryValType::INT_32_ARRAY_1D}, //U_INT_32,
    {u8"::int4[]",  GpDbQueryValType::INT_32_ARRAY_1D}, //S_INT_32,
    {u8"::int8[]",  GpDbQueryValType::INT_64_ARRAY_1D}, //U_INT_64,
    {u8"::int8[]",  GpDbQueryValType::INT_64_ARRAY_1D}, //S_INT_64,
    {u8"::float8[]",GpDbQueryValType::DOUBLE_ARRAY_1D}, //DOUBLE,
    {u8"::float4[]",GpDbQueryValType::FLOAT_ARRAY_1D},  //FLOAT,
    {u8"::boolean[]",GpDbQueryValType::BOOLEAN_ARRAY_1D},//BOOLEAN,
    {u8"::uuid[]",  GpDbQueryValType::UUID_ARRAY_1D},   //UUID,
    {u8"::text[]",  GpDbQueryValType::STRING_ARRAY_1D}, //STRING,
    {u8"::bytea[]", GpDbQueryValType::BLOB_ARRAY_1D},   //BLOB,
    {u8"",          GpDbQueryValType::NULL_VAL},        //OBJECT,
    {u8"::jsonb[]", GpDbQueryValType::JSON_ARRAY_1D},   //OBJECT_SP,
    {u8"",          GpDbQueryValType::NULL_VAL},        //ENUM,
    {u8"",          GpDbQueryValType::NULL_VAL}         //ENUM_FLAGS
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

GpDbQueryBuilder&   GpDbQueryBuilder::RAW (std::u8string_view aRawStr)
{
    _CheckForSpace();

    iQueryStr
        .append(aRawStr);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BRACE_BEGIN (void)
{
    iQueryStr
        .append(u8"("_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BRACE_END (void)
{
    iQueryStr
        .append(u8")"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COMMA (void)
{
    iQueryStr
        .append(u8","_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::AND (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"AND"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::OR (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"OR"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::NOT (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"NOT"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BITWISE_AND (void)
{
    iQueryStr
        .append(u8"&"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BITWISE_OR (void)
{
    iQueryStr
        .append(u8"|"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::EQUAL (void)
{
    iQueryStr
        .append(u8"="_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::NOT_EQUAL (void)
{
    iQueryStr
        .append(u8"!="_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::GREATER (void)
{
    iQueryStr
        .append(u8">"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::GREATER_EQUAL (void)
{
    iQueryStr
        .append(u8">="_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LESS (void)
{
    iQueryStr
        .append(u8"<"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LESS_EQUAL (void)
{
    iQueryStr
        .append(u8"<="_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ASSIGN (void)
{
    iQueryStr
        .append(u8"="_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ADD (void)
{
    iQueryStr
        .append(u8"+"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SUB (void)
{
    iQueryStr
        .append(u8"-"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::MUL (void)
{
    iQueryStr
        .append(u8"*"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::DIV (void)
{
    iQueryStr
        .append(u8"/"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COUNT_1 (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"COUNT(1)"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COALESCE_BEGIN (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"COALESCE("_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COALESCE_END (void)
{
    iQueryStr
        .append(u8")"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ASC (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"ASC"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::DESC (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"DESC"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::NULLS (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"NULLS"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LAST (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"LAST"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ON (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"ON"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::AS (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"AS"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::AS (std::u8string_view aName)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"AS "_sv);

    _AppendName(aName);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::IN (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"IN"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ANY
(
    const GpDbQueryValType::EnumT   aValueType,
    std::u8string_view              aTypeCast
)
{
    _CheckForSpace();

    iQueryStr.append(u8"ANY("_sv);
    VALUE(aValueType);
    iQueryStr.append(u8"::"_sv).append(aTypeCast).append(u8")"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::BETWEEN (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"BETWEEN"_sv);

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
        .append(u8"LIKE "_sv)
        .append(_ValueBind(aValueType));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LIKE (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"LIKE "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ILIKE (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"ILIKE "_sv);

    return *this;
}


GpDbQueryBuilder&   GpDbQueryBuilder::IS_NULL (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"IS NULL"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::IS_NOT_NULL (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"IS NOT NULL"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUES_BEGIN (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"VALUES("_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUES_END (void)
{
    iQueryStr
        .append(u8")"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUES (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"VALUES"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::WITH (std::u8string_view aName)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"WITH "_sv);

    _AppendName(aName);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::INSERT_INTO
(
    std::u8string_view aSchema,
    std::u8string_view aTable
)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"INSERT INTO "_sv);

    _AppendName(aSchema);

    iQueryStr
        .append(u8"."_sv);

    _AppendName(aTable);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SELECT (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"SELECT"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SELECT_ALL (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"SELECT *"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::FROM
(
    std::u8string_view aSchema,
    std::u8string_view aTable
)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"FROM "_sv);

    _AppendName(aSchema);

    iQueryStr
        .append(u8"."_sv);

    _AppendName(aTable);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::FROM (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"FROM"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::WHERE (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"WHERE"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::UPDATE
(
    std::u8string_view aSchema,
    std::u8string_view aTable
)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"UPDATE "_sv);

    _AppendName(aSchema);

    iQueryStr
        .append(u8"."_sv);

    _AppendName(aTable);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::DISTINCT (std::u8string_view aName)
{
    return RAW(u8"DISTINCT("_sv).COL(aName).RAW(u8")"_sv);
}

GpDbQueryBuilder&   GpDbQueryBuilder::SET (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"SET"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ORDER_BY (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"ORDER BY"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LIMIT (const size_t aValue)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"LIMIT "_sv)
        .append(StrOps::SToString(aValue));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LIMIT (const GpDbQueryValType::EnumT aValueType)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"LIMIT "_sv);

    VALUE(aValueType);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::OFFSET (const size_t aValue)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"OFFSET "_sv)
        .append(StrOps::SToString(aValue));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::OFFSET (const GpDbQueryValType::EnumT aValueType)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"OFFSET "_sv);

    VALUE(aValueType);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::RETURNING (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"RETURNING"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::RETURNING_ALL (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"RETURNING *"_sv);

    return *this;
}

/*GpDbQueryBuilder& GpDbQueryBuilder::RETURNING (std::u8string_view aRaw)
{
    CheckForSpace();

    iQueryStr
        .append(u8"RETURNING"_sv)
        .append(u8" "_sv);

    RAW(aRaw);

    return *this;
}*/

GpDbQueryBuilder&   GpDbQueryBuilder::FOR_UPDATE (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"FOR UPDATE"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SKIP_LOCKED (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"SKIP LOCKED"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::DELETE_FROM
(
    std::u8string_view aSchema,
    std::u8string_view aTable
)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"DELETE FROM "_sv);

    _AppendName(aSchema);

    iQueryStr
        .append(u8"."_sv);

    _AppendName(aTable);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::ON_CONFLICT (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"ON CONFLICT"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::DO_NOTHING (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"DO NOTHING"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::NOT_EXISTS (void)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"NOT EXISTS"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::INNER_JOIN
(
    std::u8string_view aSchema,
    std::u8string_view aTable
)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"INNER JOIN "_sv);

    _AppendName(aSchema);

    iQueryStr
        .append(u8"."_sv);

    _AppendName(aTable);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::LEFT_JOIN
(
    std::u8string_view aSchema,
    std::u8string_view aTable
)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"LEFT JOIN "_sv);

    _AppendName(aSchema);

    iQueryStr
        .append(u8"."_sv);

    _AppendName(aTable);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::RIGHT_JOIN
(
    std::u8string_view aSchema,
    std::u8string_view aTable
)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"RIGHT JOIN "_sv);

    _AppendName(aSchema);

    iQueryStr
        .append(u8"."_sv);

    _AppendName(aTable);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COL (std::u8string_view aName)
{
    _CheckForSpace();
    _AppendName(aName);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COL_AS
(
    std::u8string_view aName,
    std::u8string_view aNameAs
)
{
    _CheckForSpace();
    _AppendName(aName);
    AS();
    _CheckForSpace();
    _AppendName(aNameAs);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COL_EQUAL_TO_VAL
(
    std::u8string_view              aName,
    const GpDbQueryValType::EnumT   aValueType
)
{
    return COL(aName).EQUAL().VALUE(aValueType);
}

GpDbQueryBuilder&   GpDbQueryBuilder::COL_EQUAL_TO_VAL
(
    std::u8string_view  aName,
    GpDbQueryValue&&    aValue
)
{
    return COL(aName).EQUAL().VALUE(std::move(aValue));
}

GpDbQueryBuilder&   GpDbQueryBuilder::COL_EQUAL_TO_VAL
(
    std::u8string_view  aName,
    std::u8string_view  aValue
)
{
    return COL(aName).EQUAL().VALUE(aValue);
}

GpDbQueryBuilder&   GpDbQueryBuilder::COL_EQUAL_TO_COL
(
    std::u8string_view  aName1,
    std::u8string_view  aName2
)
{
    return COL(aName1).EQUAL().COL(aName2);
}

GpDbQueryBuilder&   GpDbQueryBuilder::COL_IS_NOT_NUUL (std::u8string_view aName)
{
    return COL(aName).IS_NOT_NULL();
}

GpDbQueryBuilder&   GpDbQueryBuilder::COL_IS_NUUL (std::u8string_view aName)
{
    return COL(aName).IS_NULL();
}

GpDbQueryBuilder&   GpDbQueryBuilder::COLS (const std::vector<std::u8string_view>& aNames)
{
    _CheckForSpace();

    auto getFn = [&](const auto& i) -> std::u8string
    {
        std::vector<std::u8string_view> parts = StrOps::SSplit(*i, '.', 0, 0, Algo::SplitMode::COUNT_ZERO_LENGTH_PARTS);
        std::u8string                   s;

        for (const auto& p: parts)
        {
            if (s.length() > 0)
            {
                s.append(u8"."_sv);
            }

            s.append(u8"\""_sv).append(_SCheckIfName(p)).append(u8"\""_sv);
        }

        return s;
    };

    iQueryStr
        .append(StrOps::SJoin<std::u8string>(aNames, getFn, u8","_sv))
        .append(u8" "_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::COL_ASSIGN
(
    std::u8string_view              aName,
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
        .append(u8"NULL"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE (const GpDbQueryValType::EnumT aValueType)
{
    _CheckForSpace();

    iQueryStr
        .append(_ValueBind(aValueType));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE (GpDbQueryValue&& aValue)
{
    _CheckForSpace();

    const GpDbQueryValType::EnumT valueType = SDetectQueryValType(aValue);

    iQueryStr
        .append(_ValueBind(valueType));

    iValues.emplace_back(std::move(aValue));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE
(
    const GpDbQueryValType::EnumT   aValueType,
    std::u8string_view              aTypeCast
)
{
    _CheckForSpace();

    iQueryStr
        .append(_ValueBind(aValueType, aTypeCast));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE (std::u8string_view aValue)
{
    _CheckForSpace();

    iQueryStr
        .append(u8"'"_sv)
        .append(_SEscape(aValue))
        .append(u8"'"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE
(
    std::u8string_view  aValue,
    std::u8string_view  aTypeCast
)
{
    VALUE(aValue);

    if (aTypeCast.length() > 0)
    {
        iQueryStr
            .append(u8"::"_sv).append(aTypeCast);
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
        .append(aValue.Value() ? u8"true"_sv : u8"false"_sv);

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::VALUE
(
    const GpEnum&       aValue,
    std::u8string_view  aTypeCast
)
{
    return VALUE(aValue.ToString(), aTypeCast);
}

GpDbQueryBuilder&   GpDbQueryBuilder::OBJECT_NAMES
(
    const GpReflectModel&               aModel,
    const GpDbQueryBuilderMode::EnumT   aMode,
    const s_int_32                      aLanguageId
)
{
    return OBJECT_NAMES
    (
        u8""_sv,
        aModel,
        aMode,
        aLanguageId
    );
}

GpDbQueryBuilder&   GpDbQueryBuilder::OBJECT_NAMES
(
    std::u8string_view                  aPrefix,
    const GpReflectModel&               aModel,
    const GpDbQueryBuilderMode::EnumT   aMode,
    const s_int_32                      aLanguageId
)
{
    _CheckForSpace();

    const auto info = _SFromModel(aPrefix, aModel, aMode, aLanguageId);

    auto getFn = [&](auto& i) -> std::u8string
    {
        return i->name;
    };

    iQueryStr
        .append(StrOps::SJoin<std::u8string>(info, getFn, u8","_sv));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::OBJECT_BINDS
(
    const GpReflectModel&               aModel,
    const GpDbQueryBuilderMode::EnumT   aMode,
    const s_int_32                      aRealmId,
    const s_int_32                      aLanguageId
)
{
    _CheckForSpace();

    const auto info = _SFromModel(u8""_sv, aModel, aMode, aLanguageId);

    auto getFn = [&](auto& i) -> std::u8string
    {
        iTypes.emplace_back(i->type);

        std::u8string res;
        res.reserve(16);

        const GpReflectProp& propInfo = i->propInfo;

        if (propInfo.FlagTest(GpReflectPropFlag::MULTILANGUAGE_STRING))
        {
            res
                .append(u8"language.add_item("_sv)
                .append(StrOps::SToString(aRealmId))
                .append(u8","_sv + StrOps::SToString(aLanguageId))
                .append(u8",$"_sv + StrOps::SToString<size_t>(iTypes.size()))
                .append(i->bindType)
                .append(u8")"_sv);
        } else
        {
            res
                .append(u8"$"_sv)
                .append(StrOps::SToString<size_t>(iTypes.size()))
                .append(i->bindType);
        }

        return res;
    };

    iQueryStr
        .append(StrOps::SJoin<std::u8string>(info, getFn, u8","_sv));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::OBJECT_FOR_UPDATE
(
    const GpReflectModel&               aModel,
    const GpDbQueryBuilderMode::EnumT   aMode,
    const s_int_32                      aLanguageId
)
{
    _CheckForSpace();

    const auto info = _SFromModel(u8""_sv, aModel, aMode, aLanguageId);

    auto getFn = [&](auto& i) -> std::u8string
    {
        iTypes.emplace_back(i->type);

        std::u8string res = u8"\""_sv + _SCheckIfName(i->name) + u8"\"="_sv;

        res
            .append(u8"$"_sv)
            .append(StrOps::SToString<size_t>(iTypes.size()))
            .append(i->bindType);

        return res;
    };

    iQueryStr
        .append(StrOps::SJoin<std::u8string>(info, getFn, u8","_sv));

    return *this;
}

GpDbQueryBuilder&   GpDbQueryBuilder::SEARCH_WHERE
(
    GpReflectModel::C::Opt::CRef    aModel,
    const GpDbSearchDesc&           aSearchDesc,
    const s_int_32                  aRealmId,
    const s_int_32                  aLanguageId
)
{   
    if (iQuerySearchBuilder.IsNULL())
    {
        iQuerySearchBuilder = MakeSP<GpDbQuerySearchBuilder>();
    }

    iQuerySearchBuilder.V().SEARCH_WHERE(*this, aModel, aSearchDesc, aRealmId, aLanguageId);

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

std::u8string   GpDbQueryBuilder::_ValueBind (const GpDbQueryValType::EnumT aValueType)
{
    return _ValueBind(aValueType, sBindStrs.at(size_t(aValueType)));
}

std::u8string   GpDbQueryBuilder::_ValueBind
(
    const GpDbQueryValType::EnumT   aValueType,
    std::u8string_view              aTypeCast
)
{
    iTypes.emplace_back(aValueType);

    std::u8string res;

    res
        .append(u8"$"_sv)
        .append(StrOps::SToString<size_t>(iTypes.size()));

    if (aTypeCast.length() > 0)
    {
        res.append(u8"::"_sv).append(aTypeCast);
    }

    return res;
}

void    GpDbQueryBuilder::_CheckForSpace (void)
{
    if (iQueryStr.length() > 0)
    {
        const char8_t ch = iQueryStr.at(iQueryStr.length() - 1);

        if (   (ch != ' ')
            && (ch != ')')
            && (ch != '('))
        {
            iQueryStr.append(u8" "_sv);
        }
    } else
    {
        iQueryStr.reserve(512);
    }
}

void    GpDbQueryBuilder::_AppendName (std::u8string_view aName)
{
    iQueryStr
        .append(u8"\""_sv)
        .append(_SCheckIfName(aName))
        .append(u8"\""_sv);

    std::u8string s;
}

std::u8string_view  GpDbQueryBuilder::_SCheckIfName (std::u8string_view aStr)
{
    THROW_COND_DB
    (
        aStr.length() <= 255,
        GpDbExceptionCode::QUERY_ERROR,
        "Name length must be <= 255"_sv
    );

    for (const char8_t ch: aStr)
    {
        THROW_COND_DB
        (
            ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')) || (ch == '_'),
            GpDbExceptionCode::QUERY_ERROR,
            [&](){return u8"Name '"_sv + aStr + u8"' contains wrong character '"_sv + ch + u8"'"_sv;}
        );
    }

    return aStr;
}

std::vector<GpDbQueryBuilder::TypeInfo> GpDbQueryBuilder::_SFromModel
(
    std::u8string_view                  aPrefix,
    const GpReflectModel&               aModel,
    const GpDbQueryBuilderMode::EnumT   aMode,
    const s_int_32                      aLanguageId
)
{
    std::vector<TypeInfo> res;

    const auto&     props       = aModel.Props();
    const size_t    propsCout   = props.size();

    res.reserve(propsCout);

    for (const GpReflectProp& propInfo: aModel.Props())
    {
        const GpReflectContainerType::EnumT propContainer = propInfo.Container();
        std::u8string                       valueBind;
        GpDbQueryValType::EnumT             valueType;
        std::u8string                       propName(u8"\""_sv + _SCheckIfName(propInfo.Name()) + u8"\""_sv);
        const std::u8string                 srcPropName = propName;

        if (aPrefix.length() > 0)
        {
            propName = aPrefix + u8"." + propName;
        }

        if (aMode == GpDbQueryBuilderMode::CREATE)
        {
            if (propInfo.FlagTest(GpReflectPropFlag::GENERATED_OUTSIDE))
            {
                continue;
            }
        } else if (aMode == GpDbQueryBuilderMode::READ)
        {
            if (propInfo.FlagTest(GpReflectPropFlag::MULTILANGUAGE_STRING))
            {
                propName = u8"language.get_item("_sv + propName + u8","_sv + StrOps::SToString(aLanguageId) + u8") AS "_sv + srcPropName;
            }
        } else if (aMode == GpDbQueryBuilderMode::UPDATE)
        {
            if (   (propInfo.FlagTest(GpReflectPropFlag::GENERATED_ONCE))
                || (propInfo.FlagTest(GpReflectPropFlag::GENERATED_OUTSIDE))
                || (propInfo.FlagTest(GpReflectPropFlag::PRIMARY_KEY)))
            {
                continue;
            }
        }

        switch (propContainer)
        {
            case GpReflectContainerType::NO:
            {
                std::tie(valueBind, valueType) = sTypeBind.at(NumOps::SConvert<size_t>(GpEnum::value_type(propInfo.Type())));
            } break;
            case GpReflectContainerType::VECTOR:
            {
                std::tie(valueBind, valueType) = sTypeBindVec.at(NumOps::SConvert<size_t>(GpEnum::value_type(propInfo.Type())));
            } break;
            case GpReflectContainerType::MAP:
            {
                //TODO: implement
                THROW_GP_NOT_IMPLEMENTED();
                //std::tie(valueBind, valueType) = sTypeBindMap.at(int(propInfo.Type()));
            } break;
            default:
            {
                THROW_GP(u8"Unknown container type "_sv + GpReflectContainerType::SToString(propContainer));
            }
        }

        res.emplace_back
        (
            std::move(propName),
            std::move(valueBind),
            std::move(valueType),
            propInfo
        );
    }

    return res;
}

std::u8string   GpDbQueryBuilder::_SEscape (std::u8string_view aStr)
{
    //Check if need to escape
    {
        bool isNeedToEscape = false;

        for (const char8_t ch: aStr)
        {
            if (ch == u8'\'')
            {
                isNeedToEscape = true;
                break;
            }
        }

        if (!isNeedToEscape)
        {
            return std::u8string(aStr);
        }
    }

    //Do escape
    std::u8string escapedStr;
    escapedStr.reserve(NumOps::SMul(aStr.size(), size_t(2)));

    for (const char8_t ch: aStr)
    {
        if (ch != u8'\'')
        {
            escapedStr.append(1, ch);
        } else
        {
            escapedStr.append(u8"''");
        }
    }

    return escapedStr;
}

}//GPlatform

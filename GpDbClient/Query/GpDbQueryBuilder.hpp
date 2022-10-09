#pragma once

#include "GpDbQueryValType.hpp"
#include "GpDbQueryValue.hpp"

namespace GPlatform {

class GpDbQuery;
class GpDbQuerySearchBuilder;

GP_ENUM(GP_DB_CLIENT_API, GpDbQueryBuilderMode,
    CREATE,
    READ,
    UPDATE
);

class GP_DB_CLIENT_API GpDbQueryBuilder
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbQueryBuilder)
    CLASS_DD(GpDbQueryBuilder)

    using ValueTypeVecT = std::vector<GpDbQueryValType::EnumT>;
    using ValueVecT     = std::vector<GpDbQueryValue>;
    using BindStrsT     = std::array<std::string, GpDbQueryValType::SCount()>;
    using BindCacheT    = std::array<std::tuple<std::string, GpDbQueryValType::EnumT>, GpDbQueryValType::SCount()>;

    struct TypeInfo
    {
                                TypeInfo (std::string&&                 aName,
                                          std::string&&                 aBindType,
                                          const GpDbQueryValType::EnumT aType) noexcept:name(std::move(aName)), bindType(std::move(aBindType)), type(aType) {}

        std::string             name;       //*values names*/
        std::string             bindType;   //::type
        GpDbQueryValType::EnumT type;
    };

public:
                                GpDbQueryBuilder    (void) noexcept;
                                ~GpDbQueryBuilder   (void) noexcept;

    std::string_view            QueryStr            (void) const noexcept {return iQueryStr;}
    const ValueTypeVecT&        Types               (void) const noexcept {return iTypes;}
    const ValueVecT&            Values              (void) const noexcept {return iValues;}

    std::string&&               QueryStrMove        (void) noexcept {return std::move(iQueryStr);}
    ValueTypeVecT&&             TypesMove           (void) noexcept {return std::move(iTypes);}
    ValueVecT&&                 ValuesMove          (void) noexcept {return std::move(iValues);}

    GpDbQueryBuilder&           RAW                 (std::string_view aRawStr);

    GpDbQueryBuilder&           BRACE_BEGIN         (void);
    GpDbQueryBuilder&           BRACE_END           (void);
    GpDbQueryBuilder&           COMMA               (void);
    GpDbQueryBuilder&           AND                 (void);
    GpDbQueryBuilder&           OR                  (void);
    GpDbQueryBuilder&           NOT                 (void);
    GpDbQueryBuilder&           BITWISE_AND         (void);
    GpDbQueryBuilder&           BITWISE_OR          (void);
    GpDbQueryBuilder&           EQUAL               (void);
    GpDbQueryBuilder&           NOT_EQUAL           (void);
    GpDbQueryBuilder&           GREATER             (void);
    GpDbQueryBuilder&           GREATER_EQUAL       (void);
    GpDbQueryBuilder&           LESS                (void);
    GpDbQueryBuilder&           LESS_EQUAL          (void);
    GpDbQueryBuilder&           ASSIGN              (void);
    GpDbQueryBuilder&           ADD                 (void);
    GpDbQueryBuilder&           SUB                 (void);
    GpDbQueryBuilder&           MUL                 (void);
    GpDbQueryBuilder&           DIV                 (void);
    GpDbQueryBuilder&           COUNT_1             (void);
    GpDbQueryBuilder&           COALESCE_BEGIN      (void);
    GpDbQueryBuilder&           COALESCE_END        (void);
    GpDbQueryBuilder&           ASC                 (void);
    GpDbQueryBuilder&           DESC                (void);
    GpDbQueryBuilder&           NULLS               (void);
    GpDbQueryBuilder&           LAST                (void);
    GpDbQueryBuilder&           ON                  (void);
    GpDbQueryBuilder&           AS                  (void);
    GpDbQueryBuilder&           AS                  (std::string_view aName);
    GpDbQueryBuilder&           IN                  (void);
    GpDbQueryBuilder&           ANY                 (const GpDbQueryValType::EnumT  aValueType,
                                                     std::string_view               aTypeCast);
    GpDbQueryBuilder&           BETWEEN             (void);
    GpDbQueryBuilder&           BETWEEN             (const GpDbQueryValType::EnumT aValueTypeA,
                                                     const GpDbQueryValType::EnumT aValueTypeB);
    GpDbQueryBuilder&           LIKE                (const GpDbQueryValType::EnumT aValueType);
    GpDbQueryBuilder&           IS_NULL             (void);
    GpDbQueryBuilder&           IS_NOT_NULL         (void);

    GpDbQueryBuilder&           VALUES_BEGIN        (void);
    GpDbQueryBuilder&           VALUES_END          (void);
    GpDbQueryBuilder&           VALUES              (void);

    GpDbQueryBuilder&           WITH                (std::string_view aName);
    GpDbQueryBuilder&           INSERT_INTO         (std::string_view aSchema,
                                                     std::string_view aTable);
    GpDbQueryBuilder&           SELECT              (void);
    GpDbQueryBuilder&           SELECT_ALL          (void);
    GpDbQueryBuilder&           FROM                (std::string_view aSchema,
                                                     std::string_view aTable);
    GpDbQueryBuilder&           FROM                (void);
    GpDbQueryBuilder&           WHERE               (void);
    GpDbQueryBuilder&           UPDATE              (std::string_view aSchema,
                                                     std::string_view aTable);
    GpDbQueryBuilder&           SET                 (void);
    GpDbQueryBuilder&           ORDER_BY            (void);
    GpDbQueryBuilder&           LIMIT               (const size_t aValue);
    GpDbQueryBuilder&           LIMIT               (const GpDbQueryValType::EnumT aValueType);
    GpDbQueryBuilder&           OFFSET              (const size_t aValue);
    GpDbQueryBuilder&           OFFSET              (const GpDbQueryValType::EnumT aValueType);
    GpDbQueryBuilder&           RETURNING           (void);
    GpDbQueryBuilder&           RETURNING_ALL       (void);
    GpDbQueryBuilder&           FOR_UPDATE          (void);
    GpDbQueryBuilder&           SKIP_LOCKED         (void);
    GpDbQueryBuilder&           DELETE_FROM         (std::string_view aSchema,
                                                     std::string_view aTable);
    GpDbQueryBuilder&           ON_CONFLICT         (void);
    GpDbQueryBuilder&           DO_NOTHING          (void);
    GpDbQueryBuilder&           NOT_EXISTS          (void);

    GpDbQueryBuilder&           INNER_JOIN          (std::string_view aSchema,
                                                     std::string_view aTable);
    GpDbQueryBuilder&           LEFT_JOIN           (std::string_view aSchema,
                                                     std::string_view aTable);
    GpDbQueryBuilder&           RIGHT_JOIN          (std::string_view aSchema,
                                                     std::string_view aTable);

    GpDbQueryBuilder&           COL                 (std::string_view aName);
    GpDbQueryBuilder&           COL_EQUAL_TO        (std::string_view               aName,
                                                     const GpDbQueryValType::EnumT  aValueType);
    GpDbQueryBuilder&           COL_IS_NOT_NUUL     (std::string_view aName);
    GpDbQueryBuilder&           COL_IS_NUUL         (std::string_view aName);
    GpDbQueryBuilder&           COLS                (const std::vector<std::string_view>& aNames);
    GpDbQueryBuilder&           COL_ASSIGN          (std::string_view               aName,
                                                     const GpDbQueryValType::EnumT  aValueType);

    GpDbQueryBuilder&           VALUE_NULL          (void);
    GpDbQueryBuilder&           VALUE               (const GpDbQueryValType::EnumT aValueType);
    GpDbQueryBuilder&           VALUE               (const GpDbQueryValType::EnumT  aValueType,
                                                     GpDbQueryValue&&               aValue);
    GpDbQueryBuilder&           VALUE               (const GpDbQueryValType::EnumT  aValueType,
                                                     std::string_view               aTypeCast);
    GpDbQueryBuilder&           VALUE               (std::string_view aValue);
    GpDbQueryBuilder&           VALUE               (std::string_view   aValue,
                                                     std::string_view   aTypeCast);
    GpDbQueryBuilder&           VALUE               (const s_int_64 aValue);
    GpDbQueryBuilder&           VALUE               (const GpBool aValue);
    GpDbQueryBuilder&           VALUE               (const GpEnum&      aValue,
                                                     std::string_view   aTypeCast);

    GpDbQueryBuilder&           OBJECT_NAMES        (const GpReflectModel&          aModel,
                                                     const GpDbQueryBuilderMode::EnumT  aMode);
    GpDbQueryBuilder&           OBJECT_BINDS        (const GpReflectModel&          aModel,
                                                     const GpDbQueryBuilderMode::EnumT  aMode);
    GpDbQueryBuilder&           OBJECT_FOR_UPDATE   (const GpReflectModel&          aModel,
                                                     const GpDbQueryBuilderMode::EnumT  aMode);

    GpDbQueryBuilder&           SEARCH_WHERE        (const GpDbSearchDesc&  aSearchDesc);
    GpDbQueryBuilder&           SEARCH_ORDER        (const GpDbSearchDesc&  aSearchDesc);
    GpDbQueryBuilder&           SEARCH_LIMIT        (const GpDbSearchDesc&  aSearchDesc);

    GpSP<GpDbQuerySearchBuilder>QuerySearchBuilder  (void) const {return iQuerySearchBuilder;}

    template<typename T> static
    consteval GpDbQueryValType::EnumT
                                SDetectValType      (void);

private:
    std::string                 _ValueBind          (const GpDbQueryValType::EnumT aValueType);
    std::string                 _ValueBind          (const GpDbQueryValType::EnumT  aValueType,
                                                     std::string_view               aTypeCast);
    void                        _CheckForSpace      (void);
    void                        _AppendName         (std::string_view aName);
    static std::string_view     _SCheckIfName       (std::string_view aStr);
    static std::vector<TypeInfo>    _SFromModel         (const GpReflectModel&          aModel,
                                                     const GpDbQueryBuilderMode::EnumT  aMode);
    static std::string          _SEscape            (std::string_view aStr);

private:
    std::string                     iQueryStr;
    ValueTypeVecT                   iTypes;
    ValueVecT                       iValues;
    GpSP<GpDbQuerySearchBuilder>    iQuerySearchBuilder;

    static const BindStrsT          sBindStrs;
    static const BindCacheT         sTypeBind;
    static const BindCacheT         sTypeBindVec;
    static const BindCacheT         sTypeBindMap;
};

template<typename T>
consteval GpDbQueryValType::EnumT GpDbQueryBuilder::SDetectValType (void)
{
    constexpr GpReflectType::EnumT t = GpReflectUtils::SDetectType<T>();

         if constexpr (t == GpReflectType::NOT_SET)     GpThrowCe<T>("Unsupported type");
    else if constexpr (t == GpReflectType::U_INT_8)     return GpDbQueryValType::INT_16;
    else if constexpr (t == GpReflectType::S_INT_8)     return GpDbQueryValType::INT_16;
    else if constexpr (t == GpReflectType::U_INT_16)    return GpDbQueryValType::INT_16;
    else if constexpr (t == GpReflectType::S_INT_16)    return GpDbQueryValType::INT_16;
    else if constexpr (t == GpReflectType::U_INT_32)    return GpDbQueryValType::INT_32;
    else if constexpr (t == GpReflectType::S_INT_32)    return GpDbQueryValType::INT_32;
    else if constexpr (t == GpReflectType::U_INT_64)    return GpDbQueryValType::INT_64;
    else if constexpr (t == GpReflectType::S_INT_64)    return GpDbQueryValType::INT_64;
    else if constexpr (t == GpReflectType::DOUBLE)      return GpDbQueryValType::DOUBLE;
    else if constexpr (t == GpReflectType::FLOAT)       return GpDbQueryValType::FLOAT;
    else if constexpr (t == GpReflectType::BOOLEAN)     return GpDbQueryValType::BOOLEAN;
    else if constexpr (t == GpReflectType::UUID)        return GpDbQueryValType::UUID;
    else if constexpr (t == GpReflectType::STRING)      return GpDbQueryValType::STRING;
    else if constexpr (t == GpReflectType::BLOB)        return GpDbQueryValType::BLOB;
    else if constexpr (t == GpReflectType::OBJECT)      GpThrowCe<T>("Unsupported type");
    else if constexpr (t == GpReflectType::OBJECT_SP)   GpThrowCe<T>("Unsupported type");
    else if constexpr (t == GpReflectType::ENUM)        return GpDbQueryValType::STRING;
    else GpThrowCe<T>("Unsupported type");
}

}//GPlatform

#pragma once

#include "GpDbQueryValType.hpp"
#include "GpDbQueryValue.hpp"
#include "../../GpDbDataModels/Search/GpDbSearchDesc.hpp"

#include <GpCore2/GpReflection/GpReflectModel.hpp>
#include <GpCore2/GpUtils/Types/Bool/GpBool.hpp>

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
                                          const GpDbQueryValType::EnumT aType,
                                          const GpReflectProp&          aPropInfo) noexcept:
                                name(std::move(aName)),
                                bindType(std::move(aBindType)),
                                type(aType),
                                propInfo(aPropInfo)
                                {}

        std::string             name;       //*values names*/
        std::string             bindType;   //::type
        GpDbQueryValType::EnumT type;
        const GpReflectProp&    propInfo;
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
    GpDbQueryBuilder&           ANY                 (const GpDbQueryValType::EnumT  aValueType);
    GpDbQueryBuilder&           ANY                 (const GpDbQueryValType::EnumT  aValueType,
                                                     std::string_view               aTypeCast);
    GpDbQueryBuilder&           BETWEEN             (void);
    GpDbQueryBuilder&           BETWEEN             (const GpDbQueryValType::EnumT aValueTypeA,
                                                     const GpDbQueryValType::EnumT aValueTypeB);
    GpDbQueryBuilder&           LIKE                (const GpDbQueryValType::EnumT aValueType);
    GpDbQueryBuilder&           LIKE                (void);
    GpDbQueryBuilder&           ILIKE               (void);
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
    GpDbQueryBuilder&           SELECT_ALL          (std::string_view aTable);
    GpDbQueryBuilder&           FROM                (std::string_view aTable);
    GpDbQueryBuilder&           FROM                (std::string_view aSchema,
                                                     std::string_view aTable);
    GpDbQueryBuilder&           FROM                (void);
    GpDbQueryBuilder&           WHERE               (void);
    GpDbQueryBuilder&           UPDATE              (std::string_view aSchema,
                                                     std::string_view aTable);
    GpDbQueryBuilder&           DISTINCT            (std::string_view aName);
    GpDbQueryBuilder&           DISTINCT            (std::string_view aTable,
                                                     std::string_view aName);
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
    GpDbQueryBuilder&           COL                 (std::string_view aTable,
                                                     std::string_view aName);
    GpDbQueryBuilder&           COL_AS              (std::string_view aName,
                                                     std::string_view aNameAs);
    GpDbQueryBuilder&           COL_AS              (std::string_view aTable,
                                                     std::string_view aName,
                                                     std::string_view aNameAs);
    GpDbQueryBuilder&           COL_EQUAL_TO_VAL    (std::string_view               aName,
                                                     const GpDbQueryValType::EnumT  aValueType);
    GpDbQueryBuilder&           COL_EQUAL_TO_VAL    (std::string_view   aName,
                                                     GpDbQueryValue&&   aValue);
    GpDbQueryBuilder&           COL_EQUAL_TO_VAL    (std::string_view   aName,
                                                     std::string_view   aValue);
    GpDbQueryBuilder&           COL_EQUAL_TO_COL    (std::string_view   aName1,
                                                     std::string_view   aName2);

    GpDbQueryBuilder&           COL_IS_NOT_NUUL     (std::string_view aName);
    GpDbQueryBuilder&           COL_IS_NUUL         (std::string_view aName);
    GpDbQueryBuilder&           COLS                (const std::vector<std::string_view>& aNames);
    GpDbQueryBuilder&           COL_ASSIGN          (std::string_view               aName,
                                                     const GpDbQueryValType::EnumT  aValueType);

    GpDbQueryBuilder&           NAME                (std::string_view aName);

    GpDbQueryBuilder&           VALUE_NULL          (void);
    GpDbQueryBuilder&           VALUE               (const GpDbQueryValType::EnumT aValueType);
    GpDbQueryBuilder&           VALUE               (GpDbQueryValue&& aValue);
    GpDbQueryBuilder&           VALUE               (const GpDbQueryValType::EnumT  aValueType,
                                                     std::string_view               aTypeCast);
    GpDbQueryBuilder&           VALUE               (std::string_view aValue);
    GpDbQueryBuilder&           VALUE               (std::string_view   aValue,
                                                     std::string_view   aTypeCast);
    GpDbQueryBuilder&           VALUE               (const s_int_64 aValue);
    GpDbQueryBuilder&           VALUE               (const GpBool aValue);
    GpDbQueryBuilder&           VALUE               (const GpEnum&      aValue,
                                                     std::string_view   aTypeCast);

    GpDbQueryBuilder&           OBJECT_NAMES        (const GpReflectModel&              aModel,
                                                     const GpDbQueryBuilderMode::EnumT  aMode);
    GpDbQueryBuilder&           OBJECT_NAMES        (std::string_view                   aPrefix,
                                                     const GpReflectModel&              aModel,
                                                     const GpDbQueryBuilderMode::EnumT  aMode);
    GpDbQueryBuilder&           OBJECT_BINDS        (const GpReflectModel&              aModel,
                                                     const GpDbQueryBuilderMode::EnumT  aMode);
    GpDbQueryBuilder&           OBJECT_FOR_UPDATE   (const GpReflectModel&              aModel);

    GpDbQueryBuilder&           SEARCH_WHERE        (GpReflectModel::C::Opt::CRef   aModel,
                                                     const GpDbSearchDesc&          aSearchDesc);
    GpDbQueryBuilder&           SEARCH_ORDER        (const GpDbSearchDesc&          aSearchDesc);
    GpDbQueryBuilder&           SEARCH_LIMIT        (const GpDbSearchDesc&          aSearchDesc);

    GpSP<GpDbQuerySearchBuilder>QuerySearchBuilder  (void) const {return iQuerySearchBuilder;}

    inline GpDbQueryValType::EnumT
                                SDetectQueryValType (const GpDbQueryValue& aValue);

private:
    std::string                 _ValueBind          (const GpDbQueryValType::EnumT aValueType);
    std::string                 _ValueBind          (const GpDbQueryValType::EnumT  aValueType,
                                                     std::string_view               aTypeCast);
    void                        _CheckForSpace      (void);
    void                        _AppendName         (std::string_view aName);
    static std::string_view     _SCheckIfName       (std::string_view aStr);
    static std::vector<TypeInfo>_SFromModel         (std::string_view                   aPrefix,
                                                     const GpReflectModel&              aModel,
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
    static const BindCacheT         sTypeBindVecWrap;
    static const BindCacheT         sTypeBindMap;
};

GpDbQueryValType::EnumT GpDbQueryBuilder::SDetectQueryValType (const GpDbQueryValue& aValue)
{
    return GpDbQueryValType::EnumT(aValue.index());
}

}// namespace GPlatform

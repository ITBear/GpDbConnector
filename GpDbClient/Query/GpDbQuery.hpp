#pragma once

#include <GpDbConnector/GpDbClient/Query/GpDbQueryBuilder.hpp>

namespace GPlatform {

class GpDbQueryMapperCacheValue;

class GP_DB_CLIENT_API GpDbQuery
{
public:
    CLASS_REMOVE_CTRS_COPY(GpDbQuery)
    CLASS_DD(GpDbQuery)

    using TypeVecT  = std::vector<GpDbQueryValType::EnumT>;
    using ValueVecT = std::vector<GpDbQueryValue>;

public:
    inline                              GpDbQuery           (void) noexcept;
    inline                              GpDbQuery           (GpDbQuery&& aDbQuery) noexcept;
    inline                              GpDbQuery           (std::string aQueryStr) noexcept;
    inline                              GpDbQuery           (std::string_view aQueryStr);
    inline                              GpDbQuery           (std::string        aQueryStr,
                                                             const TypeVecT&    aTypes);
    inline                              GpDbQuery           (std::string&&  aQueryStr,
                                                             TypeVecT&&     aTypes) noexcept;
    inline                              GpDbQuery           (GpCSP<GpDbQueryMapperCacheValue>&& aCacheValueCSP);
                                        GpDbQuery           (const GpDbQueryMapperCacheValue& aCacheValue);
    inline                              GpDbQuery           (GpDbQueryBuilder&& aQueryBuilder);
                                        ~GpDbQuery          (void) noexcept;

    inline GpDbQuery&                   operator=           (GpDbQuery&& aDbQuery) noexcept;

    std::string_view                    QueryStr            (void) const noexcept {return iQueryStr;}
    std::string                         ValuesToStr         (void) const;
    const TypeVecT&                     Types               (void) const noexcept {return iTypes;}
    const ValueVecT&                    Values              (void) const noexcept {return iValues;}

    void                                SetValues           (GpDbQuery&& aDbQuery);
    void                                AddValues           (GpDbQuery&& aDbQuery);
    void                                Reset               (void);

    GpDbQuery&                          NextInt16           (const s_int_16                 aValue);
    GpDbQuery&                          NextInt16Array1D    (const std::vector<s_int_16>&   aValue);
    GpDbQuery&                          NextInt16Array1D    (std::vector<s_int_16>&&        aValue);

    GpDbQuery&                          InsertInt16         (const size_t                   aId,
                                                             const s_int_16                 aValue);
    GpDbQuery&                          InsertInt16Array1D  (const size_t                   aId,
                                                             const std::vector<s_int_16>&   aValue);
    GpDbQuery&                          InsertInt16Array1D  (const size_t                   aId,
                                                             std::vector<s_int_16>&&        aValue);

    GpDbQuery&                          NextInt32           (const s_int_32                 aValue);
    GpDbQuery&                          NextInt32Array1D    (const std::vector<s_int_32>&   aValue);
    GpDbQuery&                          NextInt32Array1D    (std::vector<s_int_32>&&        aValue);

    GpDbQuery&                          InsertInt32         (const size_t                   aId,
                                                             const s_int_32                 aValue);
    GpDbQuery&                          InsertInt32Array1D  (const size_t                   aId,
                                                             const std::vector<s_int_32>&   aValue);
    GpDbQuery&                          InsertInt32Array1D  (const size_t                   aId,
                                                             std::vector<s_int_32>&&        aValue);

    GpDbQuery&                          NextInt64           (const s_int_64                 aValue);
    GpDbQuery&                          NextInt64Array1D    (const std::vector<s_int_64>&   aValue);
    GpDbQuery&                          NextInt64Array1D    (std::vector<s_int_64>&&        aValue);

    GpDbQuery&                          NextDouble          (const double                   aValue);
    GpDbQuery&                          NextDoubleArray1D   (const std::vector<double>&     aValue);
    GpDbQuery&                          NextDoubleArray1D   (std::vector<double>&&          aValue);

    GpDbQuery&                          NextFloat           (const float                    aValue);
    GpDbQuery&                          NextFloatArray1D    (const std::vector<float>&      aValue);
    GpDbQuery&                          NextFloatArray1D    (std::vector<float>&&           aValue);

    GpDbQuery&                          NextStr             (std::string_view                       aValue);
    GpDbQuery&                          NextStr             (std::string&&                          aValue);
    GpDbQuery&                          NextStrArray1D      (const std::vector<std::string_view>&   aValue);
    GpDbQuery&                          NextStrArray1D      (const std::vector<std::string>&        aValue);
    GpDbQuery&                          NextStrArray1D      (std::vector<std::string>&&             aValue);
    GpDbQuery&                          NextStrArray1D      (const GpEnumFlags&                     aValue);

    GpDbQuery&                          NextJson            (std::string_view                       aValue);
    GpDbQuery&                          NextJson            (std::string&&                          aValue);
    GpDbQuery&                          NextJson            (const GpReflectObject&                 aValue);
    GpDbQuery&                          NextJsonArray1D     (const std::vector<std::string_view>&   aValue);
    GpDbQuery&                          NextJsonArray1D     (const std::vector<std::string>&        aValue);
    GpDbQuery&                          NextJsonArray1D     (std::vector<std::string>&&             aValue);
    GpDbQuery&                          NextJsonArray1D     (const std::vector<GpReflectObject::SP>& aValue);
    GpDbQuery&                          NextJsonArray1D     (const GpVectorReflectObjWrapBase&      aValue);

    GpDbQuery&                          NextUuid            (const GpUUID&                      aValue);
    GpDbQuery&                          NextUuidArray1D     (const std::vector<GpUUID>&         aValue);
    GpDbQuery&                          NextUuidArray1D     (std::vector<GpUUID>&&              aValue);

    GpDbQuery&                          NextBlob            (const GpSpanByteR                  aValue);
    GpDbQuery&                          NextBlob            (GpBytesArray&&                     aValue);
    GpDbQuery&                          NextBlobArray1D     (const std::vector<GpBytesArray>&   aValue);
    GpDbQuery&                          NextBlobArray1D     (std::vector<GpBytesArray>&&        aValue);

    GpDbQuery&                          NextBoolean         (const bool                 aValue);

    GpDbQuery&                          NextNULL            (void);

    GpDbQuery&                          NextProp            (const GpReflectProp&   aProp,
                                                             const void*            aDataPtr);

    GpDbQuery&                          AddInt16            (const s_int_16                 aValue);
    GpDbQuery&                          AddInt16Array1D     (const std::vector<s_int_16>&   aValue);
    GpDbQuery&                          AddInt16Array1D     (std::vector<s_int_16>&&        aValue);

    GpDbQuery&                          AddInt32            (const s_int_32                 aValue);
    GpDbQuery&                          AddInt32Array1D     (const std::vector<s_int_32>&   aValue);
    GpDbQuery&                          AddInt32Array1D     (std::vector<s_int_32>&&        aValue);

    GpDbQuery&                          AddInt64            (const s_int_64                 aValue);
    GpDbQuery&                          AddInt64Array1D     (const std::vector<s_int_64>&   aValue);
    GpDbQuery&                          AddInt64Array1D     (std::vector<s_int_64>&&        aValue);

    GpDbQuery&                          AddDouble           (const double                   aValue);
    GpDbQuery&                          AddDoubleArray1D    (const std::vector<double>&     aValue);
    GpDbQuery&                          AddDoubleArray1D    (std::vector<double>&&          aValue);

    GpDbQuery&                          AddFloat            (const float                    aValue);
    GpDbQuery&                          AddFloatArray1D     (const std::vector<float>&      aValue);
    GpDbQuery&                          AddFloatArray1D     (std::vector<float>&&           aValue);

    GpDbQuery&                          AddStr              (std::string_view                       aValue);
    GpDbQuery&                          AddStr              (std::string&&                          aValue);
    GpDbQuery&                          AddStrArray1D       (const std::vector<std::string_view>&   aValue);
    GpDbQuery&                          AddStrArray1D       (const std::vector<std::string>&        aValue);
    GpDbQuery&                          AddStrArray1D       (std::vector<std::string>&&             aValue);
    GpDbQuery&                          AddStrArray1D       (const GpEnumFlags&                     aValue);

    GpDbQuery&                          AddJson             (std::string_view                           aValue);
    GpDbQuery&                          AddJson             (std::string&&                              aValue);
    GpDbQuery&                          AddJson             (const GpReflectObject&                     aValue);
    GpDbQuery&                          AddJsonArray1D      (const std::vector<std::string_view>&       aValue);
    GpDbQuery&                          AddJsonArray1D      (const std::vector<std::string>&            aValue);
    GpDbQuery&                          AddJsonArray1D      (std::vector<std::string>&&                 aValue);
    GpDbQuery&                          AddJsonArray1D      (const std::vector<GpReflectObject::SP>&    aValue);

    GpDbQuery&                          AddUuid             (const GpUUID&                      aValue);
    GpDbQuery&                          AddUuidArray1D      (const std::vector<GpUUID>&         aValue);
    GpDbQuery&                          AddUuidArray1D      (std::vector<GpUUID>&&              aValue);

    GpDbQuery&                          AddBlob             (const GpSpanByteR                  aValue);
    GpDbQuery&                          AddBlob             (GpBytesArray&&                     aValue);
    GpDbQuery&                          AddBlobArray1D      (const std::vector<GpBytesArray>&   aValue);
    GpDbQuery&                          AddBlobArray1D      (std::vector<GpBytesArray>&&        aValue);

    GpDbQuery&                          AddBoolean          (const bool                         aValue);
    GpDbQuery&                          AddBooleanArray1D   (const std::vector<bool>&           aValue);
    GpDbQuery&                          AddBooleanArray1D   (std::vector<bool>&&                aValue);

    GpDbQuery&                          AddNULL             (void);

    GpDbQuery&                          FromObject          (const GpReflectObject&         aObject,
                                                             GpDbQueryBuilderMode::EnumT    aMode);

    inline s_int_16                     Int16               (const size_t aId) const;
    inline const std::vector<s_int_16>& Int16Array1D        (const size_t aId) const;
    inline s_int_32                     Int32               (const size_t aId) const;
    inline const std::vector<s_int_32>& Int32Array1D        (const size_t aId) const;
    inline s_int_64                     Int64               (const size_t aId) const;
    inline const std::vector<s_int_64>& Int64Array1D        (const size_t aId) const;
    inline double                       Double              (const size_t aId) const;
    inline const std::vector<double>&   DoubleArray1D       (const size_t aId) const;
    inline float                        Float               (const size_t aId) const;
    inline const std::vector<float>&    FloatArray1D        (const size_t aId) const;
    inline std::string_view             Str                 (const size_t aId) const;
    inline const std::vector<std::string>&StrArray1D        (const size_t aId) const;
    inline std::string_view             Json                (const size_t aId) const;
    inline const std::vector<std::string>&JsonArray1D       (const size_t aId) const;
    inline const GpUUID&                Uuid                (const size_t aId) const;
    inline const std::vector<GpUUID>&   UuidArray1D         (const size_t aId) const;
    inline bool                         Boolean             (const size_t aId) const;
    inline const std::vector<bool>&     BooleanArray1D      (const size_t aId) const;
    inline const GpBytesArray&          Blob                (const size_t aId) const;
    inline const std::vector<GpBytesArray>&BlobArray1D      (const size_t aId) const;

private:
    void                                _NextProp           (const GpReflectProp&   aProp,
                                                             const void*            aDataPtr);
    void                                _NextPropVec        (const GpReflectProp&   aProp,
                                                             const void*            aDataPtr);
    void                                _NextPropVecWrap    (const GpReflectProp&   aProp,
                                                             const void*            aDataPtr);
    void                                _NextPropMap        (const GpReflectProp&   aProp,
                                                             const void*            aDataPtr);

    template<GpDbQueryValType::EnumT E>
    void                                _Next               (const ssize_t  aId,
                                                             auto&&         aValue);

    template<GpDbQueryValType::EnumT E>
    void                                _Add                (auto&& aValue);

    template<typename T>
    std::vector<T>                      _MakeArray          (const auto& aVector) const;

    template<typename T>
    std::vector<T>                      _MakeNumArray       (const auto& aVector) const;

private:
    std::string         iQueryStr;
    TypeVecT            iTypes;
    ValueVecT           iValues;
};

GpDbQuery::GpDbQuery (void) noexcept
{
}

GpDbQuery::GpDbQuery (GpDbQuery&& aDbQuery) noexcept:
iQueryStr(std::move(aDbQuery.iQueryStr)),
iTypes   (std::move(aDbQuery.iTypes)),
iValues  (std::move(aDbQuery.iValues))
{
}

GpDbQuery::GpDbQuery (std::string aQueryStr) noexcept:
iQueryStr(std::move(aQueryStr))
{
}

GpDbQuery::GpDbQuery (std::string_view aQueryStr):
iQueryStr(std::string_view(aQueryStr))
{
}

GpDbQuery::GpDbQuery
(
    std::string     aQueryStr,
    const TypeVecT& aTypes
):
iQueryStr(std::move(aQueryStr)),
iTypes(aTypes)
{
}

GpDbQuery::GpDbQuery
(
    std::string&&   aQueryStr,
    TypeVecT&&      aTypes
) noexcept:
iQueryStr{std::move(aQueryStr)},
iTypes   {std::move(aTypes)}
{
}

GpDbQuery::GpDbQuery (GpCSP<GpDbQueryMapperCacheValue>&& aCacheValueCSP):
GpDbQuery{aCacheValueCSP.Vn()}
{
}

GpDbQuery::GpDbQuery (GpDbQueryBuilder&& aQueryBuilder):
iQueryStr{aQueryBuilder.QueryStrMove()},
iTypes   {aQueryBuilder.TypesMove()},
iValues  {aQueryBuilder.ValuesMove()}
{
}

GpDbQuery&  GpDbQuery::operator= (GpDbQuery&& aDbQuery) noexcept
{
    iQueryStr   = std::move(aDbQuery.iQueryStr);
    iTypes      = std::move(aDbQuery.iTypes);
    iValues     = std::move(aDbQuery.iValues);

    return *this;
}

s_int_16    GpDbQuery::Int16 (const size_t aId) const
{
    return std::get<s_int_16>(iValues.at(aId));
}

const std::vector<s_int_16>&    GpDbQuery::Int16Array1D (const size_t aId) const
{
    return std::get<std::vector<s_int_16>>(iValues.at(aId));
}

s_int_32    GpDbQuery::Int32 (const size_t aId) const
{
    return std::get<s_int_32>(iValues.at(aId));
}

const std::vector<s_int_32>&    GpDbQuery::Int32Array1D (const size_t aId) const
{
    return std::get<std::vector<s_int_32>>(iValues.at(aId));
}

s_int_64    GpDbQuery::Int64 (const size_t aId) const
{
    return std::get<s_int_64>(iValues.at(aId));
}

const std::vector<s_int_64>&    GpDbQuery::Int64Array1D (const size_t aId) const
{
    return std::get<std::vector<s_int_64>>(iValues.at(aId));
}

double  GpDbQuery::Double (const size_t aId) const
{
    return std::get<double>(iValues.at(aId));
}

const std::vector<double>&  GpDbQuery::DoubleArray1D (const size_t aId) const
{
    return std::get<std::vector<double>>(iValues.at(aId));
}

float   GpDbQuery::Float (const size_t aId) const
{
    return std::get<float>(iValues.at(aId));
}

const std::vector<float>&   GpDbQuery::FloatArray1D (const size_t aId) const
{
    return std::get<std::vector<float>>(iValues.at(aId));
}

std::string_view    GpDbQuery::Str (const size_t aId) const
{
    return std::get<std::string>(iValues.at(aId));
}

const std::vector<std::string>& GpDbQuery::StrArray1D (const size_t aId) const
{
    return std::get<std::vector<std::string>>(iValues.at(aId));
}

std::string_view    GpDbQuery::Json (const size_t aId) const
{
    return std::get<std::string>(iValues.at(aId));
}

const std::vector<std::string>& GpDbQuery::JsonArray1D (const size_t aId) const
{
    return std::get<std::vector<std::string>>(iValues.at(aId));
}

const GpUUID&   GpDbQuery::Uuid (const size_t aId) const
{
    return std::get<GpUUID>(iValues.at(aId));
}

const std::vector<GpUUID>&  GpDbQuery::UuidArray1D (const size_t aId) const
{
    return std::get<std::vector<GpUUID>>(iValues.at(aId));
}

bool    GpDbQuery::Boolean (const size_t aId) const
{
    return std::get<bool>(iValues.at(aId));
}

const std::vector<bool>&    GpDbQuery::BooleanArray1D (const size_t aId) const
{
    return std::get<std::vector<bool>>(iValues.at(aId));
}

const GpBytesArray& GpDbQuery::Blob (const size_t aId) const
{
    return std::get<GpBytesArray>(iValues.at(aId));
}

const std::vector<GpBytesArray>&    GpDbQuery::BlobArray1D (const size_t aId) const
{
    return std::get<std::vector<GpBytesArray>>(iValues.at(aId));
}

template<GpDbQueryValType::EnumT E>
void    GpDbQuery::_Next
(
    const ssize_t   aId,
    auto&&          aValue
)
{
    const GpDbQueryValType::EnumT type = iTypes.at(std::size(iValues));

    if (type == E)
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
    } else
    {
        THROW_GP("Wrong value type, must be "_sv + GpDbQueryValType::SToString(type));
    }
}

template<GpDbQueryValType::EnumT E>
void    GpDbQuery::_Add (auto&& aValue)
{
    iTypes.emplace_back(E);

    if constexpr (std::is_move_assignable_v<decltype(aValue)>)
    {
        iValues.emplace_back(std::move(aValue));
    } else
    {
        iValues.emplace_back(aValue);
    }
}

template<typename T>
std::vector<T>  GpDbQuery::_MakeArray (const auto& aVector) const
{
    const size_t    size = std::size(aVector);
    std::vector<T>  v(size);

    for (size_t id = 0; id < size; id++)
    {
        v[id] = T(aVector[id]);
    }

    return v;
}

template<typename T>
std::vector<T>  GpDbQuery::_MakeNumArray (const auto& aVector) const
{
    const size_t    size = std::size(aVector);
    std::vector<T>  v(size);

    for (size_t id = 0; id < size; id++)
    {
        v[id] = NumOps::SConvert<T>((aVector[id]));
    }

    return v;
}

}// namespace GPlatform

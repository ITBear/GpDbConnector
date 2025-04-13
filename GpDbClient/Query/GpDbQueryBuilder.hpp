#pragma once

#include <GpDbConnector/GpDbClient/Query/GpDbQuery.hpp>

namespace GPlatform {

class GP_DB_CLIENT_API GpDbQueryBuilder
{
public:
    CLASS_REMOVE_CTRS_COPY(GpDbQueryBuilder)
    CLASS_DD(GpDbQueryBuilder)

    using ValueVecT = GpDbQuery::ValueVecT;

    /*using ValueTypeVecT   = std::vector<GpDbQueryValType::EnumT>;
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

        std::string             name;       // values names
        std::string             bindType;   //::type
        GpDbQueryValType::EnumT type;
        const GpReflectProp&    propInfo;
    };*/

public:
                            GpDbQueryBuilder    (void) noexcept;
                            GpDbQueryBuilder    (GpDbQueryBuilder&& aBuilder) noexcept;
                            ~GpDbQueryBuilder   (void) noexcept;

    GpDbQueryBuilder&       operator=           (GpDbQueryBuilder&& aBuilder) noexcept;

    GpDbQuery               Build               (void);

    // Prepared statement name
    GpDbQueryBuilder&       Name                (std::string_view aName);
    GpDbQueryBuilder&       Name                (std::string&& aName);

    // SQL query
    GpDbQueryBuilder&       Query               (const char* aQuery);
    GpDbQueryBuilder&       Query               (std::string_view aQuery);
    GpDbQueryBuilder&       Query               (std::string&& aQuery);

    // s_int_16
    GpDbQueryBuilder&       Next                (s_int_16                       aValue);
    GpDbQueryBuilder&       Next                (const std::vector<s_int_16>&   aValue);
    GpDbQueryBuilder&       Next                (std::vector<s_int_16>&&        aValue);

    // s_int_32
    GpDbQueryBuilder&       Next                (s_int_32                       aValue);
    GpDbQueryBuilder&       Next                (const std::vector<s_int_32>&   aValue);
    GpDbQueryBuilder&       Next                (std::vector<s_int_32>&&        aValue);

    // s_int_64
    GpDbQueryBuilder&       Next                (s_int_64                       aValue);
    GpDbQueryBuilder&       Next                (const std::vector<s_int_64>&   aValue);
    GpDbQueryBuilder&       Next                (std::vector<s_int_64>&&        aValue);

    // double
    GpDbQueryBuilder&       Next                (double                     aValue);
    GpDbQueryBuilder&       Next                (const std::vector<double>& aValue);
    GpDbQueryBuilder&       Next                (std::vector<double>&&      aValue);

    // float
    GpDbQueryBuilder&       Next                (float                      aValue);
    GpDbQueryBuilder&       Next                (const std::vector<float>&  aValue);
    GpDbQueryBuilder&       Next                (std::vector<float>&&       aValue);

    // std::string
    GpDbQueryBuilder&       Next                (std::string_view                       aValue);
    GpDbQueryBuilder&       Next                (std::string&&                          aValue);
    GpDbQueryBuilder&       Next                (const std::vector<std::string_view>&   aValue);
    GpDbQueryBuilder&       Next                (const std::vector<std::string>&        aValue);
    GpDbQueryBuilder&       Next                (std::vector<std::string>&&             aValue);

    // json
    GpDbQueryBuilder&       Next                (GpDbQueryValueJsonSv           aValue);
    GpDbQueryBuilder&       Next                (GpDbQueryValueJson&&           aValue);
    GpDbQueryBuilder&       Next                (const GpDbQueryValueJsonSvVec& aValue);
    GpDbQueryBuilder&       Next                (const GpDbQueryValueJsonVec&   aValue);
    GpDbQueryBuilder&       Next                (GpDbQueryValueJsonVec&&        aValue);

    // UUID
    GpDbQueryBuilder&       Next                (const GpUUID&              aValue);
    GpDbQueryBuilder&       Next                (const std::vector<GpUUID>& aValue);
    GpDbQueryBuilder&       Next                (std::vector<GpUUID>&&      aValue);

    // BLOB
    GpDbQueryBuilder&       Next                (const GpSpanByteR                  aValue);
    GpDbQueryBuilder&       Next                (GpBytesArray&&                     aValue);
    GpDbQueryBuilder&       Next                (const std::vector<GpBytesArray>&   aValue);
    GpDbQueryBuilder&       Next                (std::vector<GpBytesArray>&&        aValue);

    // bool
    GpDbQueryBuilder&       Next                (bool aValue);

    // null
    GpDbQueryBuilder&       Null                (void);

private:
    template<typename T>
    void                    Next                (ssize_t    aId,
                                                 auto&&     aValue);

    void                    ParseQuery          (void);

private:
    std::string             iName;
    std::string             iQuery;
    ValueVecT               iValues;
};

    /*
    GpDbQueryBuilder&       Next                (const GpReflectObject&                 aValue);
    GpDbQueryBuilder&       NextJsonArray1D     (const GpVectorReflectObjWrapBase&      aValue);
    GpDbQueryBuilder&       Next                (const GpEnumFlags&                     aValue);
    GpDbQueryBuilder&       NextProp            (const GpReflectProp&   aProp,
                                                 const void*            aDataPtr);

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

};

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
*/

}// namespace GPlatform

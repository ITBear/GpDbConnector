#pragma once

/*
#include <GpDbConnector/GpDbPostgreSql/GpDbPostgreSql_global.hpp>
#include <GpDbConnector/GpDbPostgreSql/GpDbArrayUtilsPgSql.hpp>
#include <GpDbConnector/GpDbClient/Query/GpDbQueryPrepared.hpp>

namespace GPlatform {

class GpDbQueryPreparedPgSql final: public GpDbQueryPrepared
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbQueryPreparedPgSql)
    CLASS_DD(GpDbQueryPreparedPgSql)

    using OIDsPtrT          = std::vector<Oid>;
    using ValuesPtrT        = std::vector<const char*>;
    using ValuesSizeT       = std::vector<int>;
    using ValuesIsBinaryT   = std::vector<int>;
    using SInt64VecT        = std::vector<s_int_64>;
    using BinaryDataVecT    = std::vector<GpBytesArray>;

public:
                            GpDbQueryPreparedPgSql  (void) noexcept;
    virtual                 ~GpDbQueryPreparedPgSql (void) noexcept override final;

    virtual void            Prepare                 (const GpDbQuery& aQuery) override final;

    const OIDsPtrT&         OIDs                    (void) const noexcept {return iOIDs;}
    const ValuesPtrT&       ValuesPtr               (void) const noexcept {return iValuesPtr;}
    const ValuesSizeT&      ValuesSize              (void) const noexcept {return iValuesSize;}
    const ValuesIsBinaryT&  ValuesIsBinary          (void) const noexcept {return iValuesIsBinary;}

private:
    void                    FillData                (const GpDbQueryValType::EnumT  aValueType,
                                                     size_t                         aValueId,
                                                     const GpDbQuery&               aQuery);

    template<typename T>
    inline void             _FillArray              (const T& aArray);

private:
    OIDsPtrT                iOIDs;
    ValuesPtrT              iValuesPtr;
    ValuesSizeT             iValuesSize;
    ValuesIsBinaryT         iValuesIsBinary;
    SInt64VecT              iSInt64Vec;
    BinaryDataVecT          iBinaryDataVec;
};

template<typename T>
void    GpDbQueryPreparedPgSql::_FillArray (const T& aArray)
{
    auto[oid, arrayData] = GpDbArrayUtilsPgSql::SBuild(aArray);

    iBinaryDataVec.emplace_back(std::move(arrayData));

    const auto& ead = iBinaryDataVec[std::size(iBinaryDataVec) - 1];

    const std::byte*    dataPtr     = reinterpret_cast<const std::byte*>(std::data(ead));
    const size_t        dataSize    = std::size(ead);

    iOIDs.emplace_back(oid);
    iValuesPtr.emplace_back(reinterpret_cast<const char*>(dataPtr));
    iValuesSize.emplace_back(NumOps::SConvert<int>(dataSize));
    iValuesIsBinary.emplace_back(1);
}

}// namespace GPlatform
*/

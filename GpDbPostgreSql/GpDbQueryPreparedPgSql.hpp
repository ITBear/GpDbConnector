#pragma once

#include "GpDbPostgreSql_global.hpp"
#include "GpDbArrayUtilsPgSql.hpp"
#include "../GpDbClient/Query/GpDbQueryPrepared.hpp"
#include <postgresql/libpq-fe.h>

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
    virtual                     ~GpDbQueryPreparedPgSql (void) noexcept override final;

    virtual void                Prepare                 (const GpDbQuery& aQuery) override final;

    const OIDsPtrT&             OIDs                    (void) const noexcept {return iOIDs;}
    const ValuesPtrT&           ValuesPtr               (void) const noexcept {return iValuesPtr;}
    const ValuesSizeT&          ValuesSize              (void) const noexcept {return iValuesSize;}
    const ValuesIsBinaryT&      ValuesIsBinary          (void) const noexcept {return iValuesIsBinary;}

private:
    void                        FillData                (const GpDbQueryValType::EnumT  aValueType,
                                                         const size_t                   aValueId,
                                                         const GpDbQuery&               aQuery);

    template<typename T>
    inline void                 _FillArray              (const T& aArray);

private:
    OIDsPtrT                    iOIDs;
    ValuesPtrT                  iValuesPtr;
    ValuesSizeT                 iValuesSize;
    ValuesIsBinaryT             iValuesIsBinary;
    SInt64VecT                  iSInt64Vec;
    BinaryDataVecT              iBinaryDataVec;
};

template<typename T>
void    GpDbQueryPreparedPgSql::_FillArray (const T& aArray)
{
    auto[oid, arrayData] = GpDbArrayUtilsPgSql::SBuild(aArray);

    const u_int_8*  dataPtr     = arrayData.data();
    const size_t    dataSize    = arrayData.size();

    iBinaryDataVec.emplace_back(std::move(arrayData));

    iOIDs.emplace_back(oid);
    iValuesPtr.emplace_back(reinterpret_cast<const char*>(dataPtr));
    iValuesSize.emplace_back(NumOps::SConvert<int>(dataSize));
    iValuesIsBinary.emplace_back(1);
}

}//namespace GPlatform

#pragma once

#include "GpDbQuery.hpp"

namespace GPlatform {

class GpDbQueryMapperCacheValue
{
public:
    CLASS_DD(GpDbQueryMapperCacheValue)

public:
                            GpDbQueryMapperCacheValue   (void) noexcept {}
                            GpDbQueryMapperCacheValue   (GpDbQueryBuilder& aBuilder) noexcept:
                                                        iTypes(std::move(aBuilder.TypesMove())),
                                                        iQuery(std::move(aBuilder.QueryStrMove())) {}
                            GpDbQueryMapperCacheValue   (GpDbQuery::TypeVecT&&  aTypes,
                                                         std::string&&          aQuery) noexcept:
                                                        iTypes(std::move(aTypes)), iQuery(std::move(aQuery)) {}
                            GpDbQueryMapperCacheValue   (const GpDbQueryMapperCacheValue& aVal):
                                                        iTypes(aVal.iTypes), iQuery(aVal.iQuery) {}
                            GpDbQueryMapperCacheValue   (GpDbQueryMapperCacheValue&& aVal) noexcept:
                                                        iTypes(std::move(aVal.iTypes)), iQuery(std::move(aVal.iQuery)) {}
                            ~GpDbQueryMapperCacheValue  (void) noexcept {}

    auto&                   operator=                   (const GpDbQueryMapperCacheValue& aVal)
                                                        {iTypes = aVal.iTypes; iQuery = aVal.iQuery; return *this;}
    auto&                   operator=                   (GpDbQueryMapperCacheValue&& aVal) noexcept
                                                        {iTypes = std::move(aVal.iTypes); iQuery = std::move(aVal.iQuery); return *this;}

public:
    GpDbQuery::TypeVecT     iTypes;
    std::string             iQuery;
};

class GP_DB_CLIENT_API GpDbQueryMapperCache
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbQueryMapperCache)
    CLASS_DD(GpDbQueryMapperCache)

public:
    using CacheT = GpElementsCatalog<GpUUID, GpDbQueryMapperCacheValue::CSP>;
    using GenFnT = std::function<GpDbQueryMapperCacheValue::CSP()>;

public:
                                        GpDbQueryMapperCache    (void) noexcept;
                                        ~GpDbQueryMapperCache   (void) noexcept;

    static GpDbQueryMapperCache&        S                       (void) noexcept {return sMapperCache;}

    const GpDbQueryMapperCacheValue&    Get                     (const GpUUID&  aUID,
                                                                 GenFnT         aGenFn);

private:
    CacheT                              iCache;

    static GpDbQueryMapperCache         sMapperCache;
};

}//GPlatform

#pragma once

#include "GpDbQuery.hpp"

#include <GpCore2/GpUtils/Types/Containers/GpDictionary.hpp>

namespace GPlatform {

// ------------------------------------ GpDbQueryMapperCacheValue -----------------------------------------

class GpDbQueryMapperCacheValue
{
public:
    CLASS_DD(GpDbQueryMapperCacheValue)

public:
                        GpDbQueryMapperCacheValue   (void) noexcept {}
                        GpDbQueryMapperCacheValue   (GpDbQueryBuilder& aBuilder) noexcept:
                                                        iTypes{std::move(aBuilder.TypesMove())},
                                                        iQuery{std::move(aBuilder.QueryStrMove())} {}
                        GpDbQueryMapperCacheValue   (GpDbQuery::TypeVecT&&  aTypes,
                                                     std::string&&          aQuery) noexcept:
                                                        iTypes{std::move(aTypes)}, iQuery(std::move(aQuery)) {}
                        GpDbQueryMapperCacheValue   (const GpDbQueryMapperCacheValue& aVal):
                                                        iTypes{aVal.iTypes}, iQuery(aVal.iQuery) {}
                        GpDbQueryMapperCacheValue   (GpDbQueryMapperCacheValue&& aVal) noexcept:
                                                        iTypes{std::move(aVal.iTypes)}, iQuery(std::move(aVal.iQuery)) {}
                        ~GpDbQueryMapperCacheValue  (void) noexcept {}

    auto&               operator=                   (const GpDbQueryMapperCacheValue& aVal)
                                                        {iTypes = aVal.iTypes; iQuery = aVal.iQuery; return *this;}
    auto&               operator=                   (GpDbQueryMapperCacheValue&& aVal) noexcept
                                                        {iTypes = std::move(aVal.iTypes); iQuery = std::move(aVal.iQuery); return *this;}

public:
    GpDbQuery::TypeVecT iTypes;
    std::string         iQuery;
};

// ------------------------------------ GpDbQueryMapperCache -----------------------------------------

class GP_DB_CLIENT_API GpDbQueryMapperCache
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbQueryMapperCache)
    CLASS_DD(GpDbQueryMapperCache)

public:
    using CacheT = GpDictionary<boost::container::flat_map<GpUUID, GpDbQueryMapperCacheValue::CSP>>;
    using GenFnT = std::function<GpDbQueryMapperCacheValue::CSP()>;

public:
                                            GpDbQueryMapperCache    (void) noexcept = default;
                                            ~GpDbQueryMapperCache   (void) noexcept = default;

    static GpDbQueryMapperCache&            S                       (void) noexcept {return sInstance;}

    inline GpDbQueryMapperCacheValue::CSP   Get                     (const GpUUID&  aUID,
                                                                     GenFnT&&       aGenFn);

private:
    CacheT                                  iCache;

    static GpDbQueryMapperCache             sInstance;
};

GpDbQueryMapperCacheValue::CSP  GpDbQueryMapperCache::Get
(
    const GpUUID&   aUID,
    GenFnT&&        aGenFn
)
{
    GpDbQueryMapperCacheValue::CSP res = iCache.GetOrGenerateNew
    (
        aUID,
        std::move(aGenFn)
    );

    return res;
}

}// namespace GPlatform

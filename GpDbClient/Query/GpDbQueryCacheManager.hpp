#pragma once

#include <GpDbConnector/GpDbClient/Query/GpDbQuery.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpSharedMap.hpp>

namespace GPlatform {

// ------------------------------------ GpDbQueryCacheValue -----------------------------------------
/*
class GpDbQueryCacheValue
{
public:
    CLASS_DD(GpDbQueryCacheValue)

public:
                            GpDbQueryCacheValue     (void) noexcept = default;
    inline                  GpDbQueryCacheValue     (GpDbQuery::TypeVecT&&  aTypes,
                                                     std::string&&          aQuery) noexcept;
    inline                  GpDbQueryCacheValue     (const GpDbQueryCacheValue& aVal);
    inline                  GpDbQueryCacheValue     (GpDbQueryCacheValue&& aVal) noexcept;
                            ~GpDbQueryCacheValue    (void) noexcept = default;

    inline auto&            operator=               (const GpDbQueryCacheValue& aVal) {iTypes = aVal.iTypes; iQuery = aVal.iQuery; return *this;}
    inline auto&            operator=               (GpDbQueryCacheValue&& aVal) noexcept {iTypes = std::move(aVal.iTypes); iQuery = std::move(aVal.iQuery); return *this;}

public:
    GpDbQuery::TypeVecT     iTypes;
    std::string             iQuery;
};

GpDbQueryCacheValue::GpDbQueryCacheValue
(
    GpDbQuery::TypeVecT&&   aTypes,
    std::string&&           aQuery
) noexcept:
iTypes{std::move(aTypes)},
iQuery{std::move(aQuery)}
{
}

GpDbQueryCacheValue::GpDbQueryCacheValue (const GpDbQueryCacheValue& aVal):
iTypes{aVal.iTypes},
iQuery{aVal.iQuery}
{
}

GpDbQueryCacheValue::GpDbQueryCacheValue (GpDbQueryCacheValue&& aVal) noexcept:
iTypes{std::move(aVal.iTypes)},
iQuery{std::move(aVal.iQuery)}
{
}

auto&   GpDbQueryCacheValue::operator= (const GpDbQueryCacheValue& aVal)
{
    iTypes = aVal.iTypes;
    iQuery = aVal.iQuery;

    return *this;
}

auto&   GpDbQueryCacheValue::operator= (GpDbQueryCacheValue&& aVal) noexcept
{
    iTypes = std::move(aVal.iTypes);
    iQuery = std::move(aVal.iQuery);

    return *this;
}*/

// ------------------------------------ GpDbQueryCacheManager -----------------------------------------
/*
class GP_DB_CLIENT_API GpDbQueryCacheManager
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbQueryCacheManager)
    CLASS_DD(GpDbQueryCacheManager)

public:
    using CacheT = GpSharedMap<boost::container::flat_map<GpUUID, GpDbQueryCacheValue::CSP>>;
    using GenFnT = std::function<GpDbQueryCacheValue::CSP()>;

public:
                                        GpDbQueryCacheManager   (void) noexcept = default;
                                        ~GpDbQueryCacheManager  (void) noexcept = default;

    static GpDbQueryCacheManager&       S                       (void) noexcept {return sInstance;}

    inline GpDbQueryCacheValue::CSP     Get                     (const GpUUID&  aUID,
                                                                 GenFnT&&       aGenFn);

private:
    CacheT                              iCache;

    static GpDbQueryCacheManager        sInstance;
};

GpDbQueryCacheValue::CSP    GpDbQueryCacheManager::Get
(
    const GpUUID&   aUID,
    GenFnT&&        aGenFn
)
{
    GpDbQueryCacheValue::CSP res = iCache.GetOrGenerateNew
    (
        aUID,
        std::move(aGenFn)
    );

    return res;
}
*/
}// namespace GPlatform

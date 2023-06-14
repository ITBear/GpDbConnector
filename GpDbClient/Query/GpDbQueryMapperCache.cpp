#include "GpDbQueryMapperCache.hpp"

namespace GPlatform {

GpDbQueryMapperCache    GpDbQueryMapperCache::sInstance;

GpDbQueryMapperCache::GpDbQueryMapperCache (void) noexcept
{
}

GpDbQueryMapperCache::~GpDbQueryMapperCache (void) noexcept
{
}

const GpDbQueryMapperCacheValue&    GpDbQueryMapperCache::Get
(
    const GpUUID&   aUID,
    GenFnT          aGenFn
)
{
    GpDbQueryMapperCacheValue::CSP& res = iCache.GetOrSet
    (
        aUID,
        aGenFn
    );

    return res.V();
}

}//namespace GPlatform

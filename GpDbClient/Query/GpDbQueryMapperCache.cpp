#include "GpDbQueryMapperCache.hpp"

namespace GPlatform {

GpDbQueryMapperCache    GpDbQueryMapperCache::sMapperCache;

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
    GpDbQueryMapperCacheValue::CSP& res = iCache.FindOrRegister
    (
        aUID,
        aGenFn
    );

    return res.V();
}

}//namespace GPlatform

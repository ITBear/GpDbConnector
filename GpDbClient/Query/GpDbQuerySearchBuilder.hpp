#pragma once

#include "GpDbQueryBuilder.hpp"
#include "GpDbQuery.hpp"

namespace GPlatform {

class GpDbQuerySearchBuilder
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbQuerySearchBuilder)
    CLASS_DD(GpDbQuerySearchBuilder)

public:
                                GpDbQuerySearchBuilder  (void) noexcept;
    virtual                     ~GpDbQuerySearchBuilder (void) noexcept;

    void                        SEARCH_WHERE            (GpDbQueryBuilder&              aBuilder,
                                                         GpReflectModel::C::Opt::CRef   aModel,
                                                         const GpDbSearchDesc&          aSearchDesc);
    void                        SEARCH_ORDER            (GpDbQueryBuilder&      aBuilder,
                                                         const GpDbSearchDesc&  aSearchDesc);
    void                        SEARCH_LIMIT            (GpDbQueryBuilder&      aBuilder,
                                                         const GpDbSearchDesc&  aSearchDesc);

private:
    void                        ProcessFilter           (GpDbQueryBuilder&              aBuilder,
                                                         GpReflectModel::C::Opt::CRef   aModel,
                                                         const GpDbSearchDesc&          aSearchDesc);
    void                        ProcessOrderByCond      (GpDbQueryBuilder&      aBuilder,
                                                         const GpDbSearchDesc&  aSearchDesc);
    void                        ProcessLimitCond        (GpDbQueryBuilder&      aBuilder,
                                                         const GpDbSearchDesc&  aSearchDesc);

private:
    size_t                      FindAndParseStr         (const size_t       aStartId,
                                                         std::u8string_view aStr,
                                                         const char8_t      aStopChar,
                                                         const char8_t      aEscapeChar,
                                                         const bool         aIsPropName,
                                                         GpDbQueryBuilder&  aBuilder);
    size_t                      ParseStr                (const size_t       aStartId,
                                                         std::u8string_view aStr,
                                                         const char8_t      aStopChar,
                                                         const char8_t      aEscapeChar,
                                                         const bool         aIsPropName,
                                                         GpDbQueryBuilder&  aBuilder);

    size_t                      ParseNum                (const size_t       aStartId,
                                                         std::u8string_view aStr,
                                                         GpDbQueryBuilder&  aBuilder);

    std::tuple<GpDbQueryValType::EnumT, size_t>
                                DetectType              (const size_t       aStartId,
                                                         std::u8string_view aStr);

private:
    std::u8string               iLastPropName;
};

}//namespace GPlatform

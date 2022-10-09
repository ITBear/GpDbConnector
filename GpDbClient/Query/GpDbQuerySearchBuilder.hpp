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
                                        GpDbQuerySearchBuilder      (void) noexcept;
    virtual                             ~GpDbQuerySearchBuilder     (void) noexcept;

    void                                SEARCH_WHERE                (GpDbQueryBuilder&      aBuilder,
                                                                     const GpDbSearchDesc&  aSearchDesc);
    void                                SEARCH_ORDER                (GpDbQueryBuilder&      aBuilder,
                                                                     const GpDbSearchDesc&  aSearchDesc);
    void                                SEARCH_LIMIT                (GpDbQueryBuilder&      aBuilder,
                                                                     const GpDbSearchDesc&  aSearchDesc);

private:
    void                                ProcessFilter               (GpDbQueryBuilder&      aBuilder,
                                                                     const GpDbSearchDesc&  aSearchDesc);
    void                                ProcessOrderByCond          (GpDbQueryBuilder&      aBuilder,
                                                                     const GpDbSearchDesc&  aSearchDesc);
    void                                ProcessLimitCond            (GpDbQueryBuilder&      aBuilder,
                                                                     const GpDbSearchDesc&  aSearchDesc);

private:
    bool                                TestChar                    (const size_t       aCharId,
                                                                     std::string_view   aStr,
                                                                     const char         aChar) const;

    size_t                              ParseStr                    (const size_t       aStartId,
                                                                     std::string_view   aStr,
                                                                     const char         aStopChar,
                                                                     const char         aEscapeChar,
                                                                     const bool         aIsName,
                                                                     GpDbQueryBuilder&  aBuilder);

    size_t                              ParseNum                    (const size_t       aStartId,
                                                                     std::string_view   aStr,
                                                                     GpDbQueryBuilder&  aBuilder);

    std::tuple<GpDbQueryValType::EnumT, size_t>
                                        DetectType                  (const size_t                   aStartId,
                                                                     std::string_view               aStr,
                                                                     const GpDbQueryValType::EnumT  aDefaultType);
};

}//namespace GPlatform

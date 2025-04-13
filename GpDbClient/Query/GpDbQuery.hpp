#pragma once

#include <GpDbConnector/GpDbClient/GpDbClient_global.hpp>
#include <GpDbConnector/GpDbClient/Query/GpDbQueryValue.hpp>

namespace GPlatform {

class GP_DB_CLIENT_API GpDbQuery
{
public:
    CLASS_DD(GpDbQuery)

    using ValueVecT = std::vector<GpDbQueryValue>;

public:
                        GpDbQuery   (void) noexcept;
                        GpDbQuery   (const GpDbQuery& aDbQuery);
                        GpDbQuery   (GpDbQuery&& aDbQuery) noexcept;
                        GpDbQuery   (std::string&&      aName,
                                     std::string&&      aQuery,
                                     ValueVecT&&        aValues) noexcept;
                        ~GpDbQuery  (void) noexcept;

    GpDbQuery&          operator=   (const GpDbQuery& aDbQuery);
    GpDbQuery&          operator=   (GpDbQuery&& aDbQuery) noexcept;

    std::string_view    Name        (void) const noexcept {return iName;}
    std::string_view    Query       (void) const noexcept {return iQuery;}
    const ValueVecT&    Values      (void) const noexcept {return iValues;}

    std::string         ToString    (void) const;

private:
    std::string         iName;
    std::string         iQuery;
    ValueVecT           iValues;
};

}// namespace GPlatform

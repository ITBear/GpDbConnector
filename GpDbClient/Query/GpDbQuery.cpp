#include <GpDbConnector/GpDbClient/Query/GpDbQuery.hpp>

namespace GPlatform {

GpDbQuery::GpDbQuery (void) noexcept
{
}

GpDbQuery::GpDbQuery (const GpDbQuery& aDbQuery):
iName  {aDbQuery.iName},
iQuery {aDbQuery.iQuery},
iValues{aDbQuery.iValues}
{
}

GpDbQuery::GpDbQuery (GpDbQuery&& aDbQuery) noexcept:
iName  {std::move(aDbQuery.iName)},
iQuery {std::move(aDbQuery.iQuery)},
iValues{std::move(aDbQuery.iValues)}
{
}

GpDbQuery::GpDbQuery
(
    std::string&&   aName,
    std::string&&   aQuery,
    ValueVecT&&     aValues
) noexcept:
iName  {std::move(aName)},
iQuery {std::move(aQuery)},
iValues{std::move(aValues)}
{
}

GpDbQuery::~GpDbQuery (void) noexcept
{
}

GpDbQuery&  GpDbQuery::operator= (const GpDbQuery& aDbQuery)
{
    iName   = aDbQuery.iName;
    iQuery  = aDbQuery.iQuery;
    iValues = aDbQuery.iValues;

    return *this;
}

GpDbQuery&  GpDbQuery::operator= (GpDbQuery&& aDbQuery) noexcept
{
    iName   = std::move(aDbQuery.iName);
    iQuery  = std::move(aDbQuery.iQuery);
    iValues = std::move(aDbQuery.iValues);

    return *this;
}

std::string GpDbQuery::ToString (void) const
{
    const size_t valuesCount = std::size(iValues);

    std::string resStr;
    resStr.reserve(1024);

    for (size_t id = 0; id < valuesCount; ++id)
    {
        const GpDbQueryValue& value = iValues.at(id);

        if (id > 0) [[likely]]
        {
            resStr.append("\n"_sv);
        }

        resStr.append(" ["_sv + (id+1));

        std::visit
        (
            [&](auto&& arg)
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, s_int_16>)
                {
                    resStr.append(" (int16)]:     "_sv).append(std::to_string(std::get<s_int_16>(value)));
                } else if constexpr (std::is_same_v<T, std::vector<s_int_16>>)
                {
                    std::string s = StrOps::SJoin<std::string>
                    (
                        std::get<std::vector<s_int_16>>(value),
                        [](const auto& i)->std::string {return std::to_string(*i);},
                        ","_sv
                    );

                    resStr.append(" (int16[])]:  ["_sv).append(s).append("]"_sv);
                } else if constexpr (std::is_same_v<T, s_int_32>)
                {
                    resStr.append(" (int32)]:     "_sv).append(std::to_string(std::get<s_int_32>(value)));
                } else if constexpr (std::is_same_v<T, std::vector<s_int_32>>)
                {
                    std::string s = StrOps::SJoin<std::string>
                    (
                        std::get<std::vector<s_int_32>>(value),
                        [](const auto& i)->std::string {return std::to_string(*i);},
                        ","_sv
                    );

                    resStr.append(" (int32[])]:  ["_sv).append(s).append("]"_sv);
                } else if constexpr (std::is_same_v<T, s_int_64>)
                {
                    resStr.append(" (int64)]:     "_sv).append(std::to_string(std::get<s_int_64>(value)));
                } else if constexpr (std::is_same_v<T, std::vector<s_int_64>>)
                {
                    std::string s = StrOps::SJoin<std::string>
                    (
                        std::get<std::vector<s_int_64>>(value),
                        [](const auto& i)->std::string {return std::to_string(*i);},
                        ","_sv
                    );

                    resStr.append(" (int64[])]:  ["_sv).append(s).append("]"_sv);
                } else if constexpr (std::is_same_v<T, double>)
                {
                    resStr.append(" (double)]:    "_sv).append(std::to_string(std::get<double>(value)));
                } else if constexpr (std::is_same_v<T, std::vector<double>>)
                {
                    std::string s = StrOps::SJoin<std::string>
                    (
                        std::get<std::vector<double>>(value),
                        [](const auto& i)->std::string {return std::to_string(*i);},
                        ","_sv
                    );

                    resStr.append(" (double[])]: ["_sv).append(s).append("]"_sv);
                } else if constexpr (std::is_same_v<T, float>)
                {
                    resStr.append(" (float)]:     "_sv).append(std::to_string(std::get<float>(value)));
                } else if constexpr (std::is_same_v<T, std::vector<float>>)
                {
                    std::string s = StrOps::SJoin<std::string>
                    (
                        std::get<std::vector<float>>(value),
                        [](const auto& i)->std::string {return std::to_string(*i);},
                        ","_sv
                    );

                    resStr.append(" (float[])]:  ["_sv).append(s).append("]"_sv);
                } else if constexpr (std::is_same_v<T, std::string>)
                {
                    resStr.append(" (str)]:      '"_sv).append(std::get<std::string>(value)).append("'"_sv);
                } else if constexpr (std::is_same_v<T, std::vector<std::string>>)
                {
                    std::string s = StrOps::SJoin<std::string>
                    (
                        std::get<std::vector<std::string>>(value),
                        [](const auto& i)->std::string {return "'"_sv + *i + "'"_sv;},
                        ","_sv
                    );

                    resStr.append(" (str[])]:    ["_sv).append(s).append("]"_sv);
                } else if constexpr (std::is_same_v<T, GpDbQueryValueJson>)
                {
                    resStr.append(" (json)]:     '"_sv).append(std::get<std::string>(value)).append("'"_sv);
                } else if constexpr (std::is_same_v<T, GpDbQueryValueJsonVec>)
                {
                    std::string s = StrOps::SJoin<std::string_view>
                    (
                        std::get<std::vector<std::string>>(value),
                        [](const auto& i)->std::string {return "'"_sv + *i + "'"_sv;},
                        ","_sv
                    );

                    resStr.append(" (json[])]:   ["_sv).append(s).append("]"_sv);
                } else if constexpr (std::is_same_v<T, GpUUID>)
                {
                    resStr.append(" (uuid)]:      "_sv).append(std::get<GpUUID>(value).ToString());
                } else if constexpr (std::is_same_v<T, std::vector<GpUUID>>)
                {
                    std::string s = StrOps::SJoin<std::string>
                    (
                        std::get<std::vector<GpUUID>>(value),
                        [](const auto& i)->std::string {return i->ToString();},
                        ","_sv
                    );

                    resStr.append(" (uuid[])]:   ["_sv).append(s).append("]"_sv);
                } else if constexpr (std::is_same_v<T, GpByteArray>)
                {
                    const GpByteArray& blob = std::get<GpByteArray>(value);
                    resStr.append(" (blob)]:      "_sv).append(StrOps::SFromBytesHex(GpSpanByteR(std::data(blob), std::size(blob))));
                } else if constexpr (std::is_same_v<T, std::vector<GpByteArray>>)
                {
                    std::string s = StrOps::SJoin<std::string>
                    (
                        std::get<std::vector<GpByteArray>>(value),
                        [](const auto& i)->std::string
                        {
                            const auto& b = *i;
                            return StrOps::SFromBytesHex(GpSpanByteR(std::data(b), std::size(b)));
                        },
                        ","_sv
                    );

                    resStr.append(" (blob[])]:   ["_sv).append(s).append("]"_sv);
                } else if constexpr (std::is_same_v<T, bool>)
                {
                    resStr.append(" (bool)]:      "_sv).append(std::get<bool>(value) ? "true"_sv : "false"_sv);
                } else if constexpr (std::is_same_v<T, std::nullopt_t>)
                {
                    resStr.append(" (null val)]:  null"_sv);
                } else
                {
                    THROW("Unknown type"_sv);
                }
            },
            value
        );
    }

    return resStr;
}

}// namespace GPlatform

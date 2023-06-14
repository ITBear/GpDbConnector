#include "GpDbArrayUtilsPgSql.hpp"

namespace GPlatform {

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<s_int_16>& aArray)
{
    constexpr const Oid INT2OID         = 21;
    constexpr const Oid INT2VECTOROID   = 22;

    return _SBuildPod<s_int_16, true>(INT2OID, INT2VECTOROID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<s_int_32>& aArray)
{
    constexpr const Oid INT4OID         = 23;
    constexpr const Oid INT4ARRAYOID    = 1007;

    return _SBuildPod<s_int_32, true>(INT4OID, INT4ARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<s_int_64>& aArray)
{
    constexpr const Oid INT8OID         = 20;
    constexpr const Oid INT8ARRAYOID    = 1016;

    return _SBuildPod<s_int_64, true>(INT8OID, INT8ARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<double>& aArray)
{
    constexpr const Oid FLOAT8OID       = 701;
    constexpr const Oid FLOAT8ARRAYOID  = 1022;

    return _SBuildPod<double, true>(FLOAT8OID, FLOAT8ARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<float>& aArray)
{
    constexpr const Oid FLOAT4OID       = 700;
    constexpr const Oid FLOAT4ARRAYOID  = 1021;

    return _SBuildPod<float, true>(FLOAT4OID, FLOAT4ARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<std::u8string>& aArray)
{
    constexpr const Oid TEXTOID         = 25;
    constexpr const Oid TEXTARRAYOID    = 1009;

    return _SBuildBytes<std::u8string>(TEXTOID, TEXTARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<GpUUID>& aArray)
{
    constexpr const Oid UUIDOID         = 2950;
    constexpr const Oid UUIDARRAYOID    = 2951;

    return _SBuildPod<GpUUID, false>(UUIDOID, UUIDARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<GpBytesArray>& aArray)
{
    constexpr const Oid BYTEAOID        = 17;
    constexpr const Oid BYTEAARRAYOID   = 1001;

    return _SBuildBytes<GpBytesArray>(BYTEAOID, BYTEAARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<bool>& aArray)
{
    constexpr const Oid BOOLOID         = 16;
    constexpr const Oid BOOLARRAYOID    = 1000;

    std::vector<std::u8string> strs;
    for (const bool v: aArray)
    {
        if (v) strs.emplace_back(u8"true");
        else strs.emplace_back(u8"false");
    }

    return _SBuildBytes<std::u8string>(BOOLOID, BOOLARRAYOID, strs);
}

}//namespace GPlatform

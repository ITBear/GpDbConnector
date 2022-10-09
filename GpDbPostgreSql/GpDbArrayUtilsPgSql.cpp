#include "GpDbArrayUtilsPgSql.hpp"

namespace GPlatform {

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<s_int_16>& aArray)
{
    constexpr int INT2OID       = 21;
    constexpr int INT2VECTOROID = 22;

    return _SBuildPod<s_int_16, true>(INT2OID, INT2VECTOROID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<s_int_32>& aArray)
{
    constexpr int INT4OID       = 23;
    constexpr int INT4ARRAYOID  = 1007;

    return _SBuildPod<s_int_32, true>(INT4OID, INT4ARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<s_int_64>& aArray)
{
    constexpr int INT8OID       = 20;
    constexpr int INT8ARRAYOID  = 1016;

    return _SBuildPod<s_int_64, true>(INT8OID, INT8ARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<double>& aArray)
{
    constexpr int FLOAT8OID     = 701;
    constexpr int FLOAT8ARRAYOID= 1022;

    return _SBuildPod<double, true>(FLOAT8OID, FLOAT8ARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<float>& aArray)
{
    constexpr int FLOAT4OID     = 700;
    constexpr int FLOAT4ARRAYOID= 1021;

    return _SBuildPod<float, true>(FLOAT4OID, FLOAT4ARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<std::string>& aArray)
{
    constexpr int TEXTOID       = 25;
    constexpr int TEXTARRAYOID  = 1009;

    return _SBuildBytes<std::string>(TEXTOID, TEXTARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<GpUUID>& aArray)
{
    constexpr int UUIDOID       = 2950;
    constexpr int UUIDARRAYOID  = 2951;

    return _SBuildPod<GpUUID, false>(UUIDOID, UUIDARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<GpBytesArray>& aArray)
{
    constexpr int BYTEAOID      = 17;
    constexpr int BYTEAARRAYOID = 1001;

    return _SBuildBytes<GpBytesArray>(BYTEAOID, BYTEAARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<bool>& aArray)
{
    constexpr int BOOLOID       = 16;
    constexpr int BOOLARRAYOID  = 1000;

    std::vector<std::string> strs;
    for (const bool v: aArray)
    {
        if (v) strs.emplace_back("true");
        else strs.emplace_back("false");
    }

    return _SBuildBytes<std::string>(BOOLOID, BOOLARRAYOID, strs);
}

}//namespace GPlatform

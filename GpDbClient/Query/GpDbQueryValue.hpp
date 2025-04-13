#pragma once

#include <GpDbConnector/GpDbClient/GpDbClient_global.hpp>

#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpBytesArray.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpTypeShell.hpp>
#include <GpCore2/GpUtils/Types/UIDs/GpUUID.hpp>
#include <variant>

namespace GPlatform {

class GpDbQueryValueJsonSv_TypeShell;
class GpDbQueryValueJsonSvVec_TypeShell;
class GpDbQueryValueJson_TypeShell;
class GpDbQueryValueJsonVec_TypeShell;

using GpDbQueryValueJsonSv      = GpTypeShell<std::string,              GpDbQueryValueJsonSv_TypeShell>;
using GpDbQueryValueJsonSvVec   = GpTypeShell<std::vector<std::string>, GpDbQueryValueJsonSvVec_TypeShell>;

using GpDbQueryValueJson        = GpTypeShell<std::string,              GpDbQueryValueJson_TypeShell>;
using GpDbQueryValueJsonVec     = GpTypeShell<std::vector<std::string>, GpDbQueryValueJsonVec_TypeShell>;

using GpDbQueryValue = std::variant
<
    s_int_16,
    std::vector<s_int_16>,
    s_int_32,
    std::vector<s_int_32>,
    s_int_64,
    std::vector<s_int_64>,
    double,
    std::vector<double>,
    float,
    std::vector<float>,
    std::string,
    std::vector<std::string>,
    GpDbQueryValueJson,
    GpDbQueryValueJsonVec,
    GpUUID,
    std::vector<GpUUID>,
    GpBytesArray,
    std::vector<GpBytesArray>,
    bool,
    std::nullopt_t
>;

}// namespace GPlatform

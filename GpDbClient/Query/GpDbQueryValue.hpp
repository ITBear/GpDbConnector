#pragma once

#include "../GpDbClient_global.hpp"

#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpBytesArray.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpTypeShell.hpp>
#include <variant>

namespace GPlatform {

class GpDbQueryValueJson_TypeShell;
class GpDbQueryValueJsonVec1D_TypeShell;

using GpDbQueryValueJson        = GpTypeShell<std::string,              GpDbQueryValueJson_TypeShell>;
using GpDbQueryValueJsonVec1D   = GpTypeShell<std::vector<std::string>, GpDbQueryValueJsonVec1D_TypeShell>;

using GpDbQueryValue = std::variant
<
    s_int_16,                   //INT_16,
    std::vector<s_int_16>,      //INT_16_ARRAY_1D,
    s_int_32,                   //INT_32,
    std::vector<s_int_32>,      //INT_32_ARRAY_1D,
    s_int_64,                   //INT_64,
    std::vector<s_int_64>,      //INT_64_ARRAY_1D,
    double,                     //DOUBLE,
    std::vector<double>,        //DOUBLE_ARRAY_1D,
    float,                      //FLOAT,
    std::vector<float>,         //FLOAT_ARRAY_1D,
    std::string,                //STRING,
    std::vector<std::string>,   //STRING_ARRAY_1D,
    GpDbQueryValueJson,         //JSON,
    GpDbQueryValueJsonVec1D,    //JSON_ARRAY_1D,
    GpUUID,                     //UUID,
    std::vector<GpUUID>,        //UUID_ARRAY_1D,
    GpBytesArray,               //BLOB,
    std::vector<GpBytesArray>,  //BLOB_ARRAY_1D,
    bool,                       //BOOLEAN,
    std::vector<bool>,          //BOOLEAN_ARRAY_1D,
    std::nullopt_t              //NULL_VAL
>;

}// namespace GPlatform

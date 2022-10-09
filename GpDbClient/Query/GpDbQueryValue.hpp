#pragma once

#include "../GpDbClient_global.hpp"

namespace GPlatform {

using GpDbQueryValue = std::variant
<
    s_int_16,                   //GpDbQueryValType::S_INT_16
    std::vector<s_int_16>,      //GpDbQueryValType::S_INT_16_ARRAY_1D
    s_int_32,                   //GpDbQueryValType::S_INT_32
    std::vector<s_int_32>,      //GpDbQueryValType::S_INT_32_ARRAY_1D
    s_int_64,                   //GpDbQueryValType::S_INT_64
    std::vector<s_int_64>,      //GpDbQueryValType::S_INT_64_ARRAY_1D
    double,                     //GpDbQueryValType::DOUBLE
    std::vector<double>,        //GpDbQueryValType::DOUBLE_ARRAY_1D
    float,                      //GpDbQueryValType::FLOAT
    std::vector<float>,         //GpDbQueryValType::FLOAT_ARRAY_1D
    std::string,                //GpDbQueryValType::STRING, GpDbQueryValType::JSON
    std::vector<std::string>,   //GpDbQueryValType::STRING_ARRAY_1D, GpDbQueryValType::JSON_ARRAY_1D
    GpUUID,                     //GpDbQueryValType::UUID
    std::vector<GpUUID>,        //GpDbQueryValType::UUID_ARRAY_1D
    GpBytesArray,               //GpDbQueryValType::BLOB
    std::vector<GpBytesArray>,  //GpDbQueryValType::BLOB_ARRAY_1D
    bool,                       //GpDbQueryValType::BOOLEAN
    std::vector<bool>,          //GpDbQueryValType::BOOLEAN_ARRAY_1D
    std::nullopt_t              //GpDbQueryValType::NULL_VAL
>;

}//namespace GPlatform

#pragma once

#include "GpDbPostgreSql_global.hpp"
#include "../../GpCore2/GpUtils/Types/Numerics/GpNumericTypes.hpp"
#include "../../GpCore2/GpUtils/Types/UIDs/GpUUID.hpp"
#include "../../GpCore2/GpUtils/Streams/GpByteWriter.hpp"
#include "../../GpCore2/GpUtils/Streams/GpByteWriterStorageByteArray.hpp"
#include "../../GpCore2/GpUtils/Macro/GpMacroClass.hpp"

#include <postgresql/libpq-fe.h>
#include <tuple>

namespace GPlatform {

struct PgArrayHeaderT
{
//  s_int_32        vl_len_;        // varlena header
    s_int_32        dimensions;
    s_int_32        dataOffset;
    Oid             oid;
    s_int_32        elementsCount;
    s_int_32        lowerBoundary;
} __attribute__ ((__packed__));/**/

//https://github.com/postgres/postgres/blob/master/src/include/utils/array.h
//https://stackoverflow.com/questions/26499266/whats-the-proper-index-for-querying-structures-in-arrays-in-postgres-jsonb/27708358#27708358
/*
* A standard varlena array has the following internal structure:
 *    <vl_len_>     - standard varlena header word
 *    <ndim>        - number of dimensions of the array
 *    <dataoffset>  - offset to stored data, or 0 if no nulls bitmap
 *    <elemtype>    - element type OID
 *    <dimensions>  - length of each array axis (C array of int)
 *    <lower bnds>  - lower boundary of each dimension (C array of int)
 *    <null bitmap> - bitmap showing locations of nulls (OPTIONAL)
 *    <actual data> - whatever is the stored data
 *
 * The <dimensions> and <lower bnds> arrays each have ndim elements.
*/

class GpDbArrayUtilsPgSql
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbArrayUtilsPgSql)

public:
    static std::tuple<Oid, GpBytesArray>    SBuild      (const std::vector<s_int_16>& aArray);
    static std::tuple<Oid, GpBytesArray>    SBuild      (const std::vector<s_int_32>& aArray);
    static std::tuple<Oid, GpBytesArray>    SBuild      (const std::vector<s_int_64>& aArray);
    static std::tuple<Oid, GpBytesArray>    SBuild      (const std::vector<double>& aArray);
    static std::tuple<Oid, GpBytesArray>    SBuild      (const std::vector<float>& aArray);
    static std::tuple<Oid, GpBytesArray>    SBuild      (const std::vector<std::u8string>& aArray);
    static std::tuple<Oid, GpBytesArray>    SBuild      (const std::vector<GpUUID>& aArray);
    static std::tuple<Oid, GpBytesArray>    SBuild      (const std::vector<GpBytesArray>& aArray);
    static std::tuple<Oid, GpBytesArray>    SBuild      (const std::vector<bool>& aArray);

private:
    template<typename T, bool IsN2H>
    static std::tuple<Oid, GpBytesArray>    _SBuildPod  (const Oid              aOid,
                                                         const Oid              aOidArray,
                                                         const std::vector<T>&  aArray);

    template<typename T>
    static std::tuple<Oid, GpBytesArray>    _SBuildBytes(const Oid              aOid,
                                                         const Oid              aOidArray,
                                                         const std::vector<T>&  aArray);
};

template<typename T, bool IsN2H>
std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::_SBuildPod
(
    const Oid               aOid,
    const Oid               aOidArray,
    const std::vector<T>&   aArray
)
{
    GpBytesArray arrayData;
    {
        const size_t elementsCount = aArray.size();

        arrayData.resize(elementsCount*sizeof(T));
        GpByteWriterStorageByteArray    arrayDataStorage(arrayData);
        GpByteWriter                    arrayDataWriter(arrayDataStorage);

        PgArrayHeaderT header;
        header.dimensions       = BitOps::H2N<s_int_32>(1);
        header.dataOffset       = BitOps::H2N<s_int_32>(0);
        header.oid              = BitOps::H2N<Oid>(aOid);
        header.elementsCount    = BitOps::H2N<s_int_32>(NumOps::SConvert<s_int_32>(elementsCount));
        header.lowerBoundary    = BitOps::H2N<s_int_32>(1);

        arrayDataWriter.Bytes({&header, 1});

        for (T e: aArray)
        {
            if constexpr (IsN2H)
            {
                e = BitOps::H2N<T>(e);
            }

            arrayDataWriter.Bytes(&e, sizeof(T));
        }

        arrayDataWriter.ShrinkToFit();
    }

    return {aOidArray, std::move(arrayData)};
}

template<typename T>
std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::_SBuildBytes
(
    const Oid               aOid,
    const Oid               aOidArray,
    const std::vector<T>&   aArray
)
{
    GpBytesArray arrayData;
    {
        arrayData.resize(512);
        GpByteWriterStorageByteArray    arrayDataStorage(arrayData);
        GpByteWriter                    arrayDataWriter(arrayDataStorage);

        PgArrayHeaderT header;
        header.dimensions       = BitOps::H2N<s_int_32>(1);
        header.dataOffset       = BitOps::H2N<s_int_32>(0);
        header.oid              = BitOps::H2N<Oid>(aOid);
        header.elementsCount    = BitOps::H2N<s_int_32>(NumOps::SConvert<s_int_32>(aArray.size()));
        header.lowerBoundary    = BitOps::H2N<s_int_32>(1);

        arrayDataWriter.Bytes({&header, 1});

        for (const T& e: aArray)
        {
            const size_t dataSize = e.size();

            //Size
            arrayDataWriter.SInt32(NumOps::SConvert<s_int_32>(dataSize));

            //Data
            arrayDataWriter.Bytes(e.data(), dataSize);
        }

        arrayDataWriter.ShrinkToFit();
    }

    return {aOidArray, std::move(arrayData)};
}

}//namespace GPlatform

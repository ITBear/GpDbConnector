#pragma once

#include "GpDbPostgreSql_global.hpp"

#include <GpCore2/GpUtils/Types/Numerics/GpNumericTypes.hpp>
#include <GpCore2/GpUtils/Types/UIDs/GpUUID.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriterStorageByteArray.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>

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

constexpr const Oid INT2OID         = 21;
constexpr const Oid INT2VECTOROID   = 22;
constexpr const Oid INT4OID         = 23;
constexpr const Oid INT4ARRAYOID    = 1007;
constexpr const Oid INT8OID         = 20;
constexpr const Oid INT8ARRAYOID    = 1016;
constexpr const Oid FLOAT8OID       = 701;
constexpr const Oid FLOAT8ARRAYOID  = 1022;
constexpr const Oid FLOAT4OID       = 700;
constexpr const Oid FLOAT4ARRAYOID  = 1021;
constexpr const Oid TEXTOID         = 25;
constexpr const Oid TEXTARRAYOID    = 1009;
constexpr const Oid UUIDOID         = 2950;
constexpr const Oid UUIDARRAYOID    = 2951;
constexpr const Oid BYTEAOID        = 17;
constexpr const Oid BYTEAARRAYOID   = 1001;
constexpr const Oid BOOLOID         = 16;
constexpr const Oid BOOLARRAYOID    = 1000;

class GpDbArrayUtilsPgSql
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbArrayUtilsPgSql)

public:
    inline static std::tuple<Oid, GpBytesArray> SBuild          (const std::vector<s_int_16>& aArray);
    inline static std::tuple<Oid, GpBytesArray> SBuild          (const std::vector<s_int_32>& aArray);
    inline static std::tuple<Oid, GpBytesArray> SBuild          (const std::vector<s_int_64>& aArray);
    inline static std::tuple<Oid, GpBytesArray> SBuild          (const std::vector<double>& aArray);
    inline static std::tuple<Oid, GpBytesArray> SBuild          (const std::vector<float>& aArray);
    inline static std::tuple<Oid, GpBytesArray> SBuild          (const std::vector<std::string>& aArray);
    inline static std::tuple<Oid, GpBytesArray> SBuild          (const std::vector<GpUUID>& aArray);
    inline static std::tuple<Oid, GpBytesArray> SBuild          (const std::vector<GpBytesArray>& aArray);
    inline static std::tuple<Oid, GpBytesArray> SBuild          (const std::vector<bool>& aArray);

    template<typename T>
    static std::vector<T>                       SRead           (GpSpanByteRW aData);

private:
    template<typename T>
    static constexpr Oid                        _SOidFromT      (void);

    template<typename T, bool IsN2H>
    static std::tuple<Oid, GpBytesArray>        _SBuildPod      (const Oid              aOid,
                                                                 const Oid              aOidArray,
                                                                 const std::vector<T>&  aArray);

    template<typename T>
    static std::tuple<Oid, GpBytesArray>        _SBuildBytes    (const Oid              aOid,
                                                                 const Oid              aOidArray,
                                                                 const std::vector<T>&  aArray);

    template<typename T, bool IsN2H>
    static std::vector<T>                       _SReadPod       (GpSpanByteR    aData,
                                                                 const size_t   aElementsCount);

    template<typename T>
    static std::vector<T>                       _SReadBytes     (GpSpanByteRW   aData,
                                                                 const size_t   aElementsCount);
};

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<s_int_16>& aArray)
{
    return _SBuildPod<s_int_16, true>(INT2OID, INT2VECTOROID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<s_int_32>& aArray)
{
    return _SBuildPod<s_int_32, true>(INT4OID, INT4ARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<s_int_64>& aArray)
{
    return _SBuildPod<s_int_64, true>(INT8OID, INT8ARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<double>& aArray)
{
    return _SBuildPod<double, true>(FLOAT8OID, FLOAT8ARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<float>& aArray)
{
    return _SBuildPod<float, true>(FLOAT4OID, FLOAT4ARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<std::string>& aArray)
{
    return _SBuildBytes<std::string>(TEXTOID, TEXTARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<GpUUID>& aArray)
{
    return _SBuildPod<GpUUID, false>(UUIDOID, UUIDARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<GpBytesArray>& aArray)
{
    return _SBuildBytes<GpBytesArray>(BYTEAOID, BYTEAARRAYOID, aArray);
}

std::tuple<Oid, GpBytesArray>   GpDbArrayUtilsPgSql::SBuild (const std::vector<bool>& aArray)
{
    std::vector<std::string> strs;
    for (const bool v: aArray)
    {
        if (v) strs.emplace_back("true");
        else strs.emplace_back("false");
    }

    return _SBuildBytes<std::string>(BOOLOID, BOOLARRAYOID, strs);
}

template<typename T>
std::vector<T>  GpDbArrayUtilsPgSql::SRead (GpSpanByteRW aData)

{
    // Check data size
    THROW_COND_GP
    (
        aData.Count() >= sizeof(PgArrayHeaderT),
        "Data size is less than PgArrayHeaderT header size"_sv
    );

    PgArrayHeaderT& header = *reinterpret_cast<PgArrayHeaderT*>(aData.Ptr());

    header.dimensions       = BitOps::N2H<s_int_32>(header.dimensions);
    header.dataOffset       = BitOps::N2H<s_int_32>(header.dataOffset);
    header.oid              = BitOps::N2H<Oid>(header.oid);
    header.elementsCount    = BitOps::N2H<s_int_32>(header.elementsCount);
    header.lowerBoundary    = BitOps::N2H<s_int_32>(header.lowerBoundary);

    // Check header.dimensions
    THROW_COND_GP
    (
        header.dimensions == 1,
        "Array must be 1D"_sv
    );

    // Check header.dataOffset
    THROW_COND_GP
    (
        header.dataOffset == 0,
        "Data offset must be 0"_sv
    );

    const Oid oid = _SOidFromT<T>();

    // Check header.oid
    THROW_COND_GP
    (
        header.oid == oid,
        [](){return "OID "_sv + std::to_string(oid) + " doesn't match type T"_sv;}
    );

    // Check header.elementsCount
    THROW_COND_GP
    (
        header.elementsCount >= 0,
        "Elements count must be 0"_sv
    );

    // Check header.lowerBoundary
    THROW_COND_GP
    (
        header.lowerBoundary == 1,
        "Lower boundary must be 1"_sv
    );

    if (header.elementsCount == 0)
    {
        return {};
    }

    aData = aData.OffsetAdd(sizeof(PgArrayHeaderT));
    const size_t elementsCount = NumOps::SConvert<size_t>(header.elementsCount);

    if constexpr(std::is_same_v<T, s_int_16>)
    {
        return _SReadPod<T, true>(aData, elementsCount);
    } else if constexpr(std::is_same_v<T, s_int_32>)
    {
        return _SReadPod<T, true>(aData, elementsCount);
    } else if constexpr(std::is_same_v<T, s_int_64>)
    {
        return _SReadPod<T, true>(aData, elementsCount);
    } else if constexpr(std::is_same_v<T, double>)
    {
        return _SReadPod<T, true>(aData, elementsCount);
    } else if constexpr(std::is_same_v<T, float>)
    {
        return _SReadPod<T, true>(aData, elementsCount);
    } else if constexpr(std::is_same_v<T, std::string_view>)
    {
        return _SReadBytes<T>(aData, elementsCount);
    } else if constexpr(std::is_same_v<T, GpSpanCharRW>)
    {
        return _SReadBytes<T>(aData, elementsCount);
    } else if constexpr(std::is_same_v<T, GpUUID>)
    {
        return _SReadPod<T, false>(aData, elementsCount);
    } else if constexpr(std::is_same_v<T, GpSpanByteR>)
    {
        return _SReadBytes<T>(aData, elementsCount);
    } else
    {
        GpThrowCe<GpException>("Unknown type");
    }

    return {};
}

template<typename T>
constexpr Oid   GpDbArrayUtilsPgSql::_SOidFromT (void)
{
    if constexpr(std::is_same_v<T, s_int_16>)
    {
        return INT2OID;
    } else if constexpr(std::is_same_v<T, s_int_32>)
    {
        return INT4OID;
    } else if constexpr(std::is_same_v<T, s_int_64>)
    {
        return INT8OID;
    } else if constexpr(std::is_same_v<T, double>)
    {
        return FLOAT8OID;
    } else if constexpr(std::is_same_v<T, float>)
    {
        return FLOAT4OID;
    } else if constexpr(std::is_same_v<T, std::string_view>)
    {
        return TEXTOID;
    } else if constexpr(std::is_same_v<T, GpSpanCharRW>)
    {
        return TEXTOID;
    } else if constexpr(std::is_same_v<T, GpUUID>)
    {
        return UUIDOID;
    } else if constexpr(std::is_same_v<T, GpSpanByteR>)
    {
        return BYTEAOID;
    } else
    {
        GpThrowCe<GpException>("Unknown type");
    }
}

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
        const size_t elementsCount = std::size(aArray);

        arrayData.resize(sizeof(PgArrayHeaderT) + elementsCount*(sizeof(s_int_32) + sizeof(T)));
        GpByteWriterStorageByteArray    arrayDataStorage(arrayData);
        GpByteWriter                    arrayDataWriter(arrayDataStorage);

        PgArrayHeaderT header;
        header.dimensions       = BitOps::H2N<s_int_32>(1);
        header.dataOffset       = BitOps::H2N<s_int_32>(0);
        header.oid              = BitOps::H2N<Oid>(aOid);
        header.elementsCount    = BitOps::H2N<s_int_32>(NumOps::SConvert<s_int_32>(elementsCount));
        header.lowerBoundary    = BitOps::H2N<s_int_32>(1);

        arrayDataWriter.Bytes({&header, 1});

        const s_int_32 sizeofElement = BitOps::H2N<s_int_32>(NumOps::SConvert<s_int_32>(sizeof(T)));

        for (T element: aArray)
        {
            if constexpr (IsN2H)
            {
                element = BitOps::H2N<T>(element);
            }

            arrayDataWriter.Bytes(&sizeofElement, sizeof(T));
            arrayDataWriter.Bytes(&element, sizeof(T));
        }

        arrayDataWriter.OnEnd();
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
        const size_t    elementsCount   = std::size(aArray);
        size_t          totalSize       = sizeof(PgArrayHeaderT) + elementsCount * sizeof(s_int_32)/*element data size*/;

        for (const T& e: aArray)
        {
            const size_t elementDataSize = std::size(e);
            totalSize += elementDataSize;
        }

        arrayData.resize(totalSize);
        GpByteWriterStorageByteArray    arrayDataStorage(arrayData);
        GpByteWriter                    arrayDataWriter(arrayDataStorage);

        PgArrayHeaderT header;
        header.dimensions       = BitOps::H2N<s_int_32>(1);
        header.dataOffset       = BitOps::H2N<s_int_32>(0);
        header.oid              = BitOps::H2N<Oid>(aOid);
        header.elementsCount    = BitOps::H2N<s_int_32>(NumOps::SConvert<s_int_32>(elementsCount));
        header.lowerBoundary    = BitOps::H2N<s_int_32>(1);

        arrayDataWriter.Bytes({&header, 1});

        for (const T& e: aArray)
        {
            const size_t elementDataSize = std::size(e);

            // Element data size
            arrayDataWriter.SI32(BitOps::H2N<s_int_32>(NumOps::SConvert<s_int_32>(elementDataSize)));

            // Element data
            arrayDataWriter.Bytes(std::data(e), elementDataSize);
        }

        arrayDataWriter.OnEnd();
    }

    return {aOidArray, std::move(arrayData)};
}

template<typename T, bool IsN2H>
std::vector<T>  GpDbArrayUtilsPgSql::_SReadPod
(
    GpSpanByteR     aData,
    const size_t    aElementsCount
)
{
    const size_t dataSize       = aData.Count();
    const size_t expectedSize   = aElementsCount * (sizeof(s_int_32) + sizeof(T));

    THROW_COND_GP
    (
        dataSize == expectedSize,
        [dataSize, expectedSize]()
        {
            return "Size "_sv + std::to_string(dataSize) + " of the result doesn't match the expected size "_sv + std::to_string(expectedSize);
        }
    );

    std::vector<T> res;
    res.resize(aElementsCount);

    for (size_t id = 0; id < aElementsCount; id++)
    {
        // Element size
        aData.OffsetAdd(sizeof(s_int_32));

        // Element value
        T element;
        std::memcpy(&element, aData.Ptr(), sizeof(T));

        if constexpr(IsN2H)
        {
            res[id] = BitOps::H2N<T>(element);
        } else
        {
            res[id] = element;
        }
    }

    return res;
}

template<typename T>
std::vector<T>  GpDbArrayUtilsPgSql::_SReadBytes
(
    GpSpanByteRW    aData,
    const size_t    aElementsCount
)
{
    const size_t dataSize   = aData.Count();
    size_t expectedSize     = aElementsCount * sizeof(s_int_32);

    {
        GpSpanByteR d = aData;
        for (size_t id = 0; id < aElementsCount; id++)
        {
            // Element size
            s_int_32 elementSize = 0;
            std::memcpy(&elementSize, d.Ptr(), sizeof(s_int_32));
            const size_t size = NumOps::SConvert<size_t>(elementSize);
            expectedSize += size;
            d.OffsetAdd(sizeof(s_int_32) + size);
        }
    }

    THROW_COND_GP
    (
        dataSize == expectedSize,
        [dataSize, expectedSize]()
        {
            return "Size "_sv + std::to_string(dataSize) + " of the result doesn't match the expected size "_sv + std::to_string(expectedSize);
        }
    );

    std::vector<T> res;
    res.resize(aElementsCount);

    for (size_t id = 0; id < aElementsCount; id++)
    {
        // Element size
        s_int_32 elementSize = 0;
        std::memcpy(&elementSize, aData.Ptr(), sizeof(s_int_32));
        const size_t size = NumOps::SConvert<size_t>(elementSize);
        aData.OffsetAdd(sizeof(s_int_32));

        // Element value
        res[id] = {reinterpret_cast<typename T::value_type*>(aData.Ptr()), size};
        aData.OffsetAdd(size);
    }

    return res;
}

}// namespace GPlatform

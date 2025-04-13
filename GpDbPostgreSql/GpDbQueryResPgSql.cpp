#include <GpDbConnector/GpDbPostgreSql/GpDbQueryResPgSql.hpp>
#include <GpDbConnector/GpDbClient/GpDbException.hpp>
#include <GpDbConnector/GpDbPostgreSql/GpDbArrayUtilsPgSql.hpp>
#include <GpCore2/GpUtils/Types/Bits/GpBitOps.hpp>
#include <GpCore2/GpUtils/Debugging/GpDebugging.hpp>

namespace GPlatform {

GpDbQueryResPgSql::GpDbQueryResPgSql (void)
{
}

GpDbQueryResPgSql::~GpDbQueryResPgSql (void) noexcept
{
}

void    GpDbQueryResPgSql::AddDataRow
(
    const PSQL::RowDescriptionDescRS&   aRowDesc,
    const PSQL::DataRowDescRS&          aRowData
)
{
    // Save aRowDesc
    if (!iRowDescOpt.has_value()) [[unlikely]]
    {
        iRowDescOpt     = aRowDesc;
        iColumnsCount   = std::size(aRowDesc.columns);
    }

    // Reserve iRowsDataVec
    {
        const size_t newSize = std::size(iRowColDataVec) + std::size(aRowData.columns);
        iRowColDataVec.reserve(newSize);
    }

    // Calculate data size
    size_t rowsColDataSize = 0;
    for (GpSpanByteRW rowData: aRowData.columns)
    {
        rowsColDataSize = NumOps::SAdd<size_t>(rowsColDataSize, rowData.Count());
    }

    // Copy data
    const size_t    oldRowColDataSize   = std::size(iRowColDataStorage);
    const size_t    newRowColDataSize   = oldRowColDataSize + rowsColDataSize;
    size_t          dataOffset          = oldRowColDataSize;

    iRowColDataStorage.resize(newRowColDataSize);
    std_byte_no_init* rowColDataStoragePtr = iRowColDataStorage.data() + oldRowColDataSize;

    for (GpSpanByteRW colData: aRowData.columns)
    {
        const size_t colDataSize = colData.Count();

        iRowColDataVec.emplace_back(dataOffset, colDataSize);
        dataOffset += colDataSize;

        std::memcpy(rowColDataStoragePtr, colData.Ptr(), colDataSize);
        rowColDataStoragePtr += colDataSize;
    }

    iRowsCount++;
}

size_t  GpDbQueryResPgSql::RowsCount (void) const
{
    return iRowsCount;
}

size_t  GpDbQueryResPgSql::ColumnsCount (void) const
{
    return iColumnsCount;
}

s_int_16    GpDbQueryResPgSql::GetInt16
(
    const size_t            aRowId,
    const size_t            aColId,
    std::optional<s_int_16> aOnNullValue
) const
{
    return SReadValue<s_int_16, s_int_16>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

std::vector<s_int_16>   GpDbQueryResPgSql::GetInt16Array1D
(
    const size_t                            aRowId,
    const size_t                            aColId,
    std::optional<std::vector<s_int_16>>    aOnNullValue
) const
{
    return SReadVector<s_int_16, s_int_16>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

s_int_32    GpDbQueryResPgSql::GetInt32
(
    const size_t            aRowId,
    const size_t            aColId,
    std::optional<s_int_32> aOnNullValue
) const
{
    return SReadValue<s_int_32, s_int_32>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

std::vector<s_int_32>   GpDbQueryResPgSql::GetInt32Array1D
(
    const size_t                            aRowId,
    const size_t                            aColId,
    std::optional<std::vector<s_int_32>>    aOnNullValue
) const
{
    return SReadVector<s_int_32, s_int_32>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

s_int_64    GpDbQueryResPgSql::GetInt64
(
    const size_t            aRowId,
    const size_t            aColId,
    std::optional<s_int_64> aOnNullValue
) const
{
    return SReadValue<s_int_64, s_int_64>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

std::vector<s_int_64>   GpDbQueryResPgSql::GetInt64Array1D
(
    const size_t                            aRowId,
    const size_t                            aColId,
    std::optional<std::vector<s_int_64>>    aOnNullValue
) const
{
    return SReadVector<s_int_64, s_int_64>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

double  GpDbQueryResPgSql::GetDouble
(
    const size_t            aRowId,
    const size_t            aColId,
    std::optional<double>   aOnNullValue
) const
{
    return SReadValue<double, double>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

std::vector<double> GpDbQueryResPgSql::GetDoubleArray1D
(
    const size_t                        aRowId,
    const size_t                        aColId,
    std::optional<std::vector<double>>  aOnNullValue
) const
{
    return SReadVector<double, double>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

float   GpDbQueryResPgSql::GetFloat
(
    const size_t            aRowId,
    const size_t            aColId,
    std::optional<float>    aOnNullValue
) const
{
    return SReadValue<float, float>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

std::vector<float>  GpDbQueryResPgSql::GetFloatArray1D
(
    const size_t                        aRowId,
    const size_t                        aColId,
    std::optional<std::vector<float>>   aOnNullValue
) const
{
    return SReadVector<float, float>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

std::string_view    GpDbQueryResPgSql::GetStr
(
    const size_t                    aRowId,
    const size_t                    aColId,
    std::optional<std::string_view> aOnNullValue
) const
{
    return SReadValue<std::string_view, std::string_view>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

std::vector<std::string_view>   GpDbQueryResPgSql::GetStrArray1D
(
    const size_t                                    aRowId,
    const size_t                                    aColId,
    std::optional<std::vector<std::string_view>>    aOnNullValue
) const
{
    return SReadVector<std::string_view, std::string_view>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

std::string_view    GpDbQueryResPgSql::GetJson
(
    const size_t                    aRowId,
    const size_t                    aColId,
    std::optional<std::string_view> aOnNullValue
) const
{
    return SReadValue<std::string_view, PSQL::TypeJsonShell>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

std::vector<std::string_view>   GpDbQueryResPgSql::GetJsonArray1D
(
    const size_t                                    aRowId,
    const size_t                                    aColId,
    std::optional<std::vector<std::string_view>>    aOnNullValue
) const
{
    return SReadVector<std::string_view, PSQL::TypeJsonShell>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

GpUUID  GpDbQueryResPgSql::GetUuid
(
    const size_t            aRowId,
    const size_t            aColId,
    std::optional<GpUUID>   aOnNullValue
) const
{
    return SReadValue<GpUUID, GpUUID>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

std::vector<GpUUID> GpDbQueryResPgSql::GetUuidArray1D
(
    const size_t                        aRowId,
    const size_t                        aColId,
    std::optional<std::vector<GpUUID>>  aOnNullValue
) const
{
    return SReadVector<GpUUID, GpUUID>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

GpSpanByteR GpDbQueryResPgSql::GetBlob
(
    const size_t                aRowId,
    const size_t                aColId,
    std::optional<GpSpanByteR>  aOnNullValue
) const
{
    return SReadValue<GpSpanByteR, GpSpanByteR>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

std::vector<GpSpanByteR>    GpDbQueryResPgSql::GetBlobArray1D
(
    const size_t                                aRowId,
    const size_t                                aColId,
    std::optional<std::vector<GpSpanByteR>> aOnNullValue
) const
{
    return SReadVector<GpSpanByteR, GpSpanByteR>(aRowId, aColId, aOnNullValue, const_cast<GpDbQueryResPgSql&>(*this));
}

bool    GpDbQueryResPgSql::GetBoolean
(
    const size_t        aRowId,
    const size_t        aColId,
    std::optional<bool> aOnNullValue
) const
{
    auto[dataPtr, columnDesc] = SRowColDataInfo(aRowId, aColId, PSQL::TypeOID::BOOL, const_cast<GpDbQueryResPgSql&>(*this));

    std::string_view str = dataPtr.AsStringView();

    if (!str.empty()) [[likely]]
    {
        const char v = str.at(0);

        return     (v == 0x01)
                || (v == 't')
                || (v == 'T')
                || (v == 'y')
                || (v == 'Y');
    }

    VERIFY
    (
        aOnNullValue.has_value(),
        [&]()
        {
            return fmt::format
            (
                "The requested column '{}':[{}][{}] value is null ",
                columnDesc.name,
                aRowId,
                aColId
            );
        }
    );

    return aOnNullValue.value();
}

GpDbQueryResPgSql::RowColDataInfoT  GpDbQueryResPgSql::SRowColDataInfo
(
    const size_t        aRowId,
    const size_t        aColId,
    const PSQL::TypeOID aTypeOID,
    GpDbQueryResPgSql&  aDbQueryRes
)
{
    const size_t rowsCount      = aDbQueryRes.iRowsCount;
    const size_t columnsCount   = aDbQueryRes.iColumnsCount;

    // Check aRowId, aColId
    VERIFY
    (
        (aRowId < rowsCount) && (aColId < columnsCount),
        GpDbExceptionCode::OUT_OF_RANGE_ROW_COL,
        [&]()
        {
            return fmt::format
            (
                "Requested column [{}][{}] is out of range [0..{}), [0..{})",
                aRowId,
                aColId,
                rowsCount,
                columnsCount
            );
        }
    );

    // Check data description
    const auto& rowDescOpt = aDbQueryRes.iRowDescOpt;

    VERIFY
    (
        rowDescOpt.has_value(),
        GpDbExceptionCode::EMPTY_RS_MODEL_DESC,
        "No data description is available for the response"
    );

    // Get column desc  
    const PSQL::RowDescriptionDescRS::ColumnDesc& columnDesc = rowDescOpt.value().columns[aColId];

    // Check OID for column
    {
        u_int_32 columnTypeOid = columnDesc.type_oid;

        // JSON and JSONB exception
        if (columnTypeOid == u_int_32(PSQL::TypeOID::JSONB)) [[unlikely]]
        {
            columnTypeOid = u_int_32(PSQL::TypeOID::JSON);
        } else if (columnTypeOid == u_int_32(PSQL::TypeOID::JSONB_ARRAY)) [[unlikely]]
        {
            columnTypeOid = u_int_32(PSQL::TypeOID::JSON_ARRAY);
        }

        if (columnTypeOid != u_int_32(aTypeOID))
        {
            GpDebugging::SBreakpoint();
        }

        // Check OID for column
        VERIFY
        (
            columnTypeOid == u_int_32(aTypeOID),
            GpDbExceptionCode::WRONG_TYPE_OID,
            [&]()
            {
                return fmt::format
                (
                    "Wrong OID type {} for column '{}':[{}][{}]. Expected type {}",
                    columnDesc.type_oid,
                    columnDesc.name,
                    aRowId,
                    aColId,
                    int(aTypeOID)
                );
            }
        );
    }

    // Get data ptr
    const auto[dataPtrOffset, dataPtrSize] = aDbQueryRes.iRowColDataVec[rowsCount*aRowId + aColId];

    return RowColDataInfoT
    {
        GpSpanByteRW
        {
            aDbQueryRes.iRowColDataStorage.data() + dataPtrOffset,
            dataPtrSize
        },
        columnDesc
    };
}

template<typename T, typename OIDT>
T   GpDbQueryResPgSql::SReadValue
(
    const size_t            aRowId,
    const size_t            aColId,
    const std::optional<T>& aOnNullValue,
    GpDbQueryResPgSql&      aDbQueryRes
)
{
    auto[dataPtr, columnDesc] = SRowColDataInfo
    (
        aRowId,
        aColId,
        PSQL::TypeOidUitls::SDetectTypeOID<OIDT>(),
        aDbQueryRes
    );

    if (!dataPtr.Empty()) [[likely]]
    {
        T value;
        if (columnDesc.format_code > 0) [[likely]] // Binary format
        {
            // TODO: implement
            THROW_NOT_IMPLEMENTED();

            //std::memcpy(&value, dataPtr.Ptr(), sizeof(value));
            //value = BitOps::N2H(value);
        } else // Text format
        {
            if constexpr (std::is_same_v<OIDT, s_int_16>)
            {
                value = NumOps::SConvert<s_int_16>(StrOps::SToSI64(dataPtr.AsStringView()));
            } else if constexpr (std::is_same_v<OIDT, s_int_32>)
            {
                value = NumOps::SConvert<s_int_32>(StrOps::SToSI64(dataPtr.AsStringView()));
            } else if constexpr (std::is_same_v<OIDT, s_int_64>)
            {
                value = StrOps::SToSI64(dataPtr.AsStringView());
            } else if constexpr (std::is_same_v<OIDT, float>)
            {
                value = float(StrOps::SToDouble(dataPtr.AsStringView()));
            } else if constexpr (std::is_same_v<OIDT, double>)
            {
                value = StrOps::SToDouble(dataPtr.AsStringView());
            } else if constexpr (std::is_same_v<OIDT, std::string_view>)
            {
                value = dataPtr.AsStringView();
            } else if constexpr (std::is_same_v<OIDT, GpUUID>)
            {
                value = GpUUID::SFromString(dataPtr.AsStringView());
            } else if constexpr (std::is_same_v<OIDT, GpSpanByteR>)
            {
                value = SReadStrToBlob(dataPtr, false);
            } else if constexpr (std::is_same_v<OIDT, PSQL::TypeJsonShell>)
            {
                value = dataPtr.AsStringView();
            } else
            {
                GpThrowCe<GpException>("Unsupported type");
            }
        }

        return value;
    }

    VERIFY
    (
        aOnNullValue.has_value(),
        [&]()
        {
            return fmt::format
            (
                "The requested column '{}':[{}][{}] value is null ",
                columnDesc.name,
                aRowId,
                aColId
            );
        }
    );

    return aOnNullValue.value();
}

template<typename T, typename OIDT>
std::vector<T>  GpDbQueryResPgSql::SReadVector
(
    const size_t                            aRowId,
    const size_t                            aColId,
    const std::optional<std::vector<T>>&    aOnNullValue,
    GpDbQueryResPgSql&                      aDbQueryRes
)
{
    auto[dataPtr, columnDesc] = SRowColDataInfo
    (
        aRowId,
        aColId,
        PSQL::TypeOidUitls::SDetectTypeOID<std::vector<OIDT>>(),
        aDbQueryRes
    );

    if (!dataPtr.Empty()) [[likely]]
    {
        std::vector<T> vecValue;
        if (columnDesc.format_code > 0) [[likely]] // Binary format
        {
            // TODO: implement
            THROW_NOT_IMPLEMENTED();

            //std::memcpy(&value, dataPtr.Ptr(), sizeof(value));
            //value = BitOps::N2H(value);
        } else // Text format
        {
            GpSpanCharRW vectorStr = dataPtr.Subspan(1, NumOps::SSub<size_t>(dataPtr.Count(), 2));

            if constexpr (std::is_same_v<OIDT, s_int_16>)
            {
                vecValue = StrOps::SToContainer<std::vector<T>>(vectorStr.AsStringView());
            } else if constexpr (std::is_same_v<OIDT, s_int_32>)
            {
                vecValue = StrOps::SToContainer<std::vector<T>>(vectorStr.AsStringView());
            } else if constexpr (std::is_same_v<OIDT, s_int_64>)
            {
                vecValue = StrOps::SToContainer<std::vector<T>>(vectorStr.AsStringView());
            } else if constexpr (std::is_same_v<OIDT, float>)
            {
                vecValue = StrOps::SToContainer<std::vector<T>>(vectorStr.AsStringView());
            } else if constexpr (std::is_same_v<OIDT, double>)
            {
                vecValue = StrOps::SToContainer<std::vector<T>>(vectorStr.AsStringView());
            } else if constexpr (std::is_same_v<OIDT, GpUUID>)
            {
                vecValue = GpUUID::SToContainer(vectorStr.AsStringView());
            } else if constexpr (std::is_same_v<OIDT, std::string_view>)
            {
                vecValue = SReadStrVector(vectorStr);
            } else if constexpr (std::is_same_v<OIDT, GpSpanByteR>)
            {
                vecValue = SReadStrToBlobVector(vectorStr);
            } else if constexpr (std::is_same_v<OIDT, PSQL::TypeJsonShell>)
            {
                vecValue = SReadStrVector(vectorStr);
            } else
            {
                GpThrowCe<GpException>("Unsupported type");
            }
        }

        return vecValue;
    }

    VERIFY
    (
        aOnNullValue.has_value(),
        [&]()
        {
            return fmt::format
            (
                "The requested column '{}':[{}][{}] value is null ",
                columnDesc.name,
                aRowId,
                aColId
            );
        }
    );

    return aOnNullValue.value();
}

std::vector<std::string_view>   GpDbQueryResPgSql::SReadStrVector (GpSpanCharRW aVectorStr)
{
    if (aVectorStr.Empty()) [[unlikely]]
    {
        return {};
    }

    enum class SearchMode
    {
        NEW_PART_NO_QUOTES,
        NEW_PART_WITH_QUOTES,
    };

    SearchMode      searchMode              = SearchMode::NEW_PART_NO_QUOTES;
    const ssize_t   strSize                 = NumOps::SConvert<ssize_t>(std::size(aVectorStr));
    char*           strPtr                  = std::data(aVectorStr);
    const char*     strPtrLastChar          = strPtr + strSize - 1;
    char*           strPtrBeginOfElement    = strPtr;

    if (*strPtr == '"') [[unlikely]]
    {
        strPtr++;
        searchMode              = SearchMode::NEW_PART_WITH_QUOTES;
        strPtrBeginOfElement    = strPtr;
    }

    std::vector<std::string_view> resVec;

    while (strPtr <= strPtrLastChar)
    {
        size_t elementSize = 0;

        if (searchMode == SearchMode::NEW_PART_NO_QUOTES) [[likely]]
        {
            // Find end of the element
            while ((strPtr <= strPtrLastChar) && (*strPtr != ','))
            {
                strPtr++;
                elementSize++;
            }
        } else// searchMode == SearchMode::NEW_PART_WITH_QUOTES
        {
            // Find end of the element
            while (strPtr <= strPtrLastChar)
            {
                const char ch = *strPtr++;
                strPtrBeginOfElement[elementSize] = ch;

                if (ch == '\\') [[unlikely]]
                {
                    if (strPtr <= strPtrLastChar)
                    {
                        strPtrBeginOfElement[elementSize] = *strPtr++;
                    }
                } else if (ch == '"') [[unlikely]]
                {
                    break;
                }

                elementSize++;
            }
        }

        // Add element to result vector
        resVec.emplace_back
        (
            std::string_view
            {
                strPtrBeginOfElement,
                elementSize
            }
        );

        // Check if end or ','
        if (strPtr <= strPtrLastChar) [[likely]]
        {
            VERIFY
            (
                *strPtr++ == ',',
                "Expected character ','"
            );

            // Update searchMode
            if (   (strPtr <= strPtrLastChar)
                && (*strPtr == '"'))
            {
                strPtr++;
                searchMode              = SearchMode::NEW_PART_WITH_QUOTES;
                strPtrBeginOfElement    = strPtr;
            } else
            {
                searchMode              = SearchMode::NEW_PART_NO_QUOTES;
                strPtrBeginOfElement    = strPtr;
            }
        }
    }

    return resVec;
}

GpSpanByteR GpDbQueryResPgSql::SReadStrToBlob
(
    GpSpanByteRW    aDataPtr,
    const bool      aIsFromArray
)
{
    std::string_view    sv              = aDataPtr.AsStringView();
    const size_t        prefixSize      = aIsFromArray ? 3 : 2;
    const std::string   expectedPrefix  = aIsFromArray ? "\\\\x" : "\\x";

    VERIFY
    (
           (std::size(sv) >= prefixSize)
        && (sv.substr(0, prefixSize) == expectedPrefix),
        [&]()
        {
            return fmt::format
            (
                "Expected size >= {} and prefix '{}'",
                prefixSize,
                expectedPrefix
            );
        }
    );

    GpBytesArray blobData = StrOps::SToBytesHex(sv.substr(prefixSize));
    std::memcpy(aDataPtr.Ptr(), std::data(blobData), std::size(blobData));

    return GpSpanByteR
    {
        aDataPtr.Ptr(),
        std::size(blobData)
    };
}

std::vector<GpSpanByteR>    GpDbQueryResPgSql::SReadStrToBlobVector (GpSpanCharRW aVectorStr)
{
    std::string_view            sv = aVectorStr.AsStringView();
    std::vector<GpSpanByteR>    resVec;

    while (true)
    {
        const size_t elementBeginId = sv.find('"');

        if (elementBeginId == std::string_view::npos) [[unlikely]]
        {
            break;
        }

        sv = sv.substr(elementBeginId + 1);

        const size_t elementEndId = sv.find('"');

        VERIFY
        (
            elementEndId != std::string_view::npos,
            "Can`t find end of BLOB string"
        );

        resVec.emplace_back
        (
            SReadStrToBlob
            (
                GpSpanByteRW
                {
                    const_cast<char*>(sv.data()),
                    elementEndId
                },
                true
            )
        );

        sv = sv.substr(elementEndId + 1);
    }

    return resVec;
}

}// namespace GPlatform

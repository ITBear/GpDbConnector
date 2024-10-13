#include <GpDbConnector/GpDbPostgreSql/GpDbQueryResPgSql.hpp>
#include <GpDbConnector/GpDbClient/GpDbException.hpp>
#include <GpDbConnector/GpDbPostgreSql/GpDbArrayUtilsPgSql.hpp>
#include <GpCore2/GpUtils/Types/Bits/GpBitOps.hpp>

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
        iColumnsCount   = aRowDesc.columns.size();
    }

    // Reserve iRowsDataVec
    const size_t rowColCount = iRowColDataVec.size() + aRowData.columns.size();
    iRowColDataVec.reserve(rowColCount);

    // Calculate data size
    size_t rowsColDataSize = 0;
    for (const GpSpanByteR& rowData: aRowData.columns)
    {
        rowsColDataSize = NumOps::SAdd<size_t>(rowsColDataSize, rowData.Count());
    }

    // Copy data
    const size_t oldRowColDataSize = iRowColDataStorage.size();
    const size_t newRowColDataSize = oldRowColDataSize + rowsColDataSize;

    iRowColDataStorage.resize(newRowColDataSize);
    std_byte_no_init*   rowColDataStoragePtr        = iRowColDataStorage.data() + oldRowColDataSize;
    const std::byte*    rowColDataStoragePtrCopy    = nullptr;

    rowColDataStoragePtrCopy += oldRowColDataSize;
    for (const GpSpanByteR& colData: aRowData.columns)
    {
        const size_t colDataSize = colData.Count();

        iRowColDataVec.emplace_back(GpSpanByteR{rowColDataStoragePtrCopy, colDataSize});
        rowColDataStoragePtrCopy += colDataSize;

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
    [[maybe_unused]] const size_t           aRowId,
    [[maybe_unused]] const size_t           aColId,
    [[maybe_unused]] std::optional<s_int_16>    aOnNullValue
) const
{
    THROW_GP_NOT_IMPLEMENTED();

    /*
    GpSpanByteR         dataPtr = RowColDataPtr(aRowId, aColId, PSQL::TypeOID::INT2);
    //std::string_view  str     = dataPtr.AsStringView();

    //if (str.data() != nullptr) [[likely]]
    //{
    //  return str;
    //}

    //THROW_COND_GP
    //(
    //  aOnNullValue.has_value(),
    //  [aRowId, aColId]()
    //  {
    //      return fmt::format
    //      (
    //          "Requested column value is null (id[{}][{}])",
    //          aRowId,
    //          aColId
    //      );
    //  }
    //);

    //return aOnNullValue.value();


    // Get column desc
    const PSQL::RowDescriptionDescRS::ColumnDesc& columnDesc = iRowDescOpt.value().columns.at(aColId);

    // Check UID
    THROW_COND_DB
    (
        columnDesc.type_oid == u_int_32(PSQL::TypeOID::INT2),
        GpDbExceptionCode::WRONG_TYPE,
        [&columnDesc]()
        {
            return fmt::format
            (
                "Type OID for 16-bit signed integer (smallint) is expected to be {}, but the actual value is {}",
                static_cast<u_int_32>(PSQL::TypeOID::INT2),
                columnDesc.type_oid
            );
        }
    );

    //iRowColDataStorage.?

    THROW_GP_NOT_IMPLEMENTED();



    //return GpDbQueryResPgSql_GetPOD<s_int_16, 2>(aRowId, aColId, aOnNullValue, iPgResult);*/
}

std::vector<s_int_16>   GpDbQueryResPgSql::GetInt16Array1D
(
    [[maybe_unused]] const size_t                           aRowId,
    [[maybe_unused]] const size_t                           aColId,
    [[maybe_unused]] std::optional<std::vector<s_int_16>>   aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //return GpDbQueryResPgSql_GetArray<s_int_16>(aRowId, aColId, aOnNullValue, iPgResult);
}

s_int_32    GpDbQueryResPgSql::GetInt32
(
    [[maybe_unused]] const size_t           aRowId,
    [[maybe_unused]] const size_t           aColId,
    [[maybe_unused]] std::optional<s_int_32>    aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //return GpDbQueryResPgSql_GetPOD<s_int_32, 4>(aRowId, aColId, aOnNullValue, iPgResult);
}

std::vector<s_int_32>   GpDbQueryResPgSql::GetInt32Array1D
(
    [[maybe_unused]] const size_t                           aRowId,
    [[maybe_unused]] const size_t                           aColId,
    [[maybe_unused]] std::optional<std::vector<s_int_32>>   aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //return GpDbQueryResPgSql_GetArray<s_int_32>(aRowId, aColId, aOnNullValue, iPgResult);
}

s_int_64    GpDbQueryResPgSql::GetInt64
(
    [[maybe_unused]] const size_t           aRowId,
    [[maybe_unused]] const size_t           aColId,
    [[maybe_unused]] std::optional<s_int_64>    aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //return GpDbQueryResPgSql_GetPOD<s_int_64, 8>(aRowId, aColId, aOnNullValue, iPgResult);
}

std::vector<s_int_64>   GpDbQueryResPgSql::GetInt64Array1D
(
    [[maybe_unused]] const size_t                           aRowId,
    [[maybe_unused]] const size_t                           aColId,
    [[maybe_unused]] std::optional<std::vector<s_int_64>>   aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //return GpDbQueryResPgSql_GetArray<s_int_64>(aRowId, aColId, aOnNullValue, iPgResult);
}

double  GpDbQueryResPgSql::GetDouble
(
    [[maybe_unused]] const size_t           aRowId,
    [[maybe_unused]] const size_t           aColId,
    [[maybe_unused]] std::optional<double>  aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //return GpDbQueryResPgSql_GetPOD<double, 8>(aRowId, aColId, aOnNullValue, iPgResult);
}

std::vector<double> GpDbQueryResPgSql::GetDoubleArray1D
(
    [[maybe_unused]] const size_t                       aRowId,
    [[maybe_unused]] const size_t                       aColId,
    [[maybe_unused]] std::optional<std::vector<double>> aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //return GpDbQueryResPgSql_GetArray<double>(aRowId, aColId, aOnNullValue, iPgResult);
}

float   GpDbQueryResPgSql::GetFloat
(
    [[maybe_unused]] const size_t           aRowId,
    [[maybe_unused]] const size_t           aColId,
    [[maybe_unused]] std::optional<float>   aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //return GpDbQueryResPgSql_GetPOD<float, 4>(aRowId, aColId, aOnNullValue, iPgResult);
}

std::vector<float>  GpDbQueryResPgSql::GetFloatArray1D
(
    [[maybe_unused]] const size_t                       aRowId,
    [[maybe_unused]] const size_t                       aColId,
    [[maybe_unused]] std::optional<std::vector<float>>  aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //return GpDbQueryResPgSql_GetArray<float>(aRowId, aColId, aOnNullValue, iPgResult);
}

std::string_view    GpDbQueryResPgSql::GetStr
(
    const size_t                    aRowId,
    const size_t                    aColId,
    std::optional<std::string_view> aOnNullValue
) const
{
    GpSpanByteR         dataPtr = RowColDataPtr(aRowId, aColId, PSQL::TypeOID::TEXT);
    std::string_view    str     = dataPtr.AsStringView();

    if (str.data() != nullptr) [[likely]]
    {
        return str;
    }

    THROW_COND_GP
    (
        aOnNullValue.has_value(),
        [aRowId, aColId]()
        {
            return fmt::format
            (
                "Requested column value is null (id[{}][{}])",
                aRowId,
                aColId
            );
        }
    );

    return aOnNullValue.value();
}

std::vector<std::string_view>   GpDbQueryResPgSql::GetStrArray1D
(
    [[maybe_unused]] const size_t                                   aRowId,
    [[maybe_unused]] const size_t                                   aColId,
    [[maybe_unused]] std::optional<std::vector<std::string_view>>   aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //return GpDbQueryResPgSql_GetArray<std::string_view>(aRowId, aColId, aOnNullValue, iPgResult);
}

GpSpanCharRW    GpDbQueryResPgSql::GetStrRW
(
    [[maybe_unused]] const size_t               aRowId,
    [[maybe_unused]] const size_t               aColId,
    [[maybe_unused]] std::optional<GpSpanCharRW>    aOnNullValue
)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    /*
    std::optional<std::string_view> defaultValue;

    if (aOnNullValue.has_value())
    {
        defaultValue = aOnNullValue.value().AsStringView();
    }

    std::string_view str = std::as_const(*this).GetStr
    (
        aRowId,
        aColId,
        defaultValue
    );

    return GpSpanCharRW(const_cast<char*>(std::data(str)), std::size(str));*/
}

std::vector<GpSpanCharRW>   GpDbQueryResPgSql::GetStrRWArray1D
(
    [[maybe_unused]] const size_t                               aRowId,
    [[maybe_unused]] const size_t                               aColId,
    [[maybe_unused]] std::optional<std::vector<GpSpanCharRW>>   aOnNullValue
)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //return GpDbQueryResPgSql_GetArray<GpSpanCharRW>(aRowId, aColId, aOnNullValue, iPgResult);
}

std::string_view    GpDbQueryResPgSql::GetJson
(
    [[maybe_unused]] const size_t                   aRowId,
    [[maybe_unused]] const size_t                   aColId,
    [[maybe_unused]] std::optional<std::string_view>    aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    /*std::string_view str = GetStr(aRowId, aColId, aOnNullValue);

    THROW_COND_GP
    (
        std::size(str) >= 3,
        "json data length must be >= 3 bytes"_sv
    );

    THROW_COND_GP
    (
        std::bit_cast<u_int_8>(std::data(str)[0]) == 1,
        "Wrong pgJson format version"_sv
    );

    return str.substr(1, std::size(str) - 1);*/
}

std::vector<std::string_view>   GpDbQueryResPgSql::GetJsonArray1D
(
    [[maybe_unused]] const size_t                                   aRowId,
    [[maybe_unused]] const size_t                                   aColId,
    [[maybe_unused]] std::optional<std::vector<std::string_view>>   aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    /*std::vector<std::string_view> strArray = GetStrArray1D(aRowId, aColId, aOnNullValue);

    for (std::string_view& element: strArray)
    {
        THROW_COND_GP
        (
            std::size(element) >= 3,
            "json data length must be >= 3 bytes"_sv
        );

        THROW_COND_GP
        (
            std::bit_cast<u_int_8>(std::data(element)[0]) == 1,
            "Wrong pgJson format version"_sv
        );

        element = element.substr(1, std::size(element) - 1);
    }

    return strArray;*/
}

GpSpanCharRW    GpDbQueryResPgSql::GetJsonRW
(
    [[maybe_unused]] const size_t               aRowId,
    [[maybe_unused]] const size_t               aColId,
    [[maybe_unused]] std::optional<GpSpanCharRW>    aOnNullValue
)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //std::optional<std::string_view> defaultValue;

    //if (aOnNullValue.has_value())
    //{
    //  defaultValue = aOnNullValue.value().AsStringView();
    //}

    //std::string_view str = std::as_const(*this).GetJson
    //(
    //  aRowId,
    //  aColId,
    //  defaultValue
    //);

    //return GpSpanCharRW(const_cast<char*>(std::data(str)), std::size(str));
}

std::vector<GpSpanCharRW>   GpDbQueryResPgSql::GetJsonRWArray1D
(
    [[maybe_unused]] const size_t                               aRowId,
    [[maybe_unused]] const size_t                               aColId,
    [[maybe_unused]] std::optional<std::vector<GpSpanCharRW>>   aOnNullValue
)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    /*
    std::vector<GpSpanCharRW> strArray = GetStrRWArray1D(aRowId, aColId, aOnNullValue);

    for (GpSpanCharRW& element: strArray)
    {
        THROW_COND_GP
        (
            element.Count() >= 3,
            "json data length must be >= 3 bytes"_sv
        );

        THROW_COND_GP
        (
            std::bit_cast<u_int_8>(element.At(0)) == 1,
            "Wrong pgJson format version"_sv
        );

        element = element.Subspan(1, element.Count() - 1);
    }

    return strArray;*/
}

GpUUID  GpDbQueryResPgSql::GetUuid
(
    [[maybe_unused]] const size_t           aRowId,
    [[maybe_unused]] const size_t           aColId,
    [[maybe_unused]] std::optional<GpUUID>  aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    /*const int rowId = NumOps::SConvert<int>(aRowId);
    const int colId = NumOps::SConvert<int>(aColId);

    if (PQgetisnull(iPgResult, rowId, colId))
    {
        THROW_COND_GP
        (
            aOnNullValue.has_value(),
            [&](){return "Value on ["_sv + aRowId + ", "_sv + aColId + "] is NULL"_sv;}
        );

        return aOnNullValue.value();
    }

    const char*     dataPtr     = PQgetvalue(iPgResult, rowId, colId);
    const size_t    dataSize    = NumOps::SConvert<size_t>(PQgetlength(iPgResult, rowId, colId));

    THROW_COND_GP
    (
        dataSize == sizeof(GpUUID::DataT),
        "uuid length must be 16 bytes"_sv
    );

    GpUUID uuid;
    std::memcpy(std::data(uuid.Data()), dataPtr, sizeof(GpUUID::DataT));

    return uuid;*/
}

std::vector<GpUUID> GpDbQueryResPgSql::GetUuidArray1D
(
    [[maybe_unused]] const size_t                       aRowId,
    [[maybe_unused]] const size_t                       aColId,
    [[maybe_unused]] std::optional<std::vector<GpUUID>> aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //return GpDbQueryResPgSql_GetArray<GpUUID>(aRowId, aColId, aOnNullValue, iPgResult);
}

GpSpanByteR GpDbQueryResPgSql::GetBlob
(
    [[maybe_unused]] const size_t               aRowId,
    [[maybe_unused]] const size_t               aColId,
    [[maybe_unused]] std::optional<GpSpanByteR> aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //const int rowId = NumOps::SConvert<int>(aRowId);
    //const int colId = NumOps::SConvert<int>(aColId);

    //if (PQgetisnull(iPgResult, rowId, colId))
    //{
    //  THROW_COND_GP
    //  (
    //      aOnNullValue.has_value(),
    //      [&](){return "Value on ["_sv + aRowId + ", "_sv + aColId + "] is NULL"_sv;}
    //  );
    //  return aOnNullValue.value();
    //}

    //const char*       dataPtr     = PQgetvalue(iPgResult, rowId, colId);
    //const size_t  dataSize    = NumOps::SConvert<size_t>(PQgetlength(iPgResult, rowId, colId));

    //return GpSpanCharR(dataPtr, dataSize);
}

std::vector<GpSpanByteR>    GpDbQueryResPgSql::GetBlobArray1D
(
    [[maybe_unused]] const size_t                           aRowId,
    [[maybe_unused]] const size_t                           aColId,
    [[maybe_unused]] std::optional<std::vector<GpSpanByteR>>    aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //return GpDbQueryResPgSql_GetArray<GpSpanByteR>(aRowId, aColId, aOnNullValue, iPgResult);
}

bool    GpDbQueryResPgSql::GetBoolean
(
    [[maybe_unused]] const size_t       aRowId,
    [[maybe_unused]] const size_t       aColId,
    [[maybe_unused]] std::optional<bool>    aOnNullValue
) const
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //const int rowId = NumOps::SConvert<int>(aRowId);
    //const int colId = NumOps::SConvert<int>(aColId);

    //if (PQgetisnull(iPgResult, rowId, colId))
    //{
    //  THROW_COND_GP
    //  (
    //      aOnNullValue.has_value(),
    //      [&](){return "Value on ["_sv + aRowId + ", "_sv + aColId + "] is NULL"_sv;}
    //  );
    //  return aOnNullValue.value();
    //}

    //const char*       dataPtr     = PQgetvalue(iPgResult, rowId, colId);
    //const size_t  dataSize    = NumOps::SConvert<size_t>(PQgetlength(iPgResult, rowId, colId));

    //std::string_view str{dataPtr, dataSize};
    //const char v = str.at(0);

    //return       (v == 0x01)
    //      || (v == 't')
    //      || (v == 'T')
    //      || (v == 'y')
    //      || (v == 'Y');
}

GpSpanByteR GpDbQueryResPgSql::RowColDataPtr
(
    const size_t        aRowId,
    const size_t        aColId,
    const PSQL::TypeOID aTypeOID
) const
{
    // Check aRowId, aColId
    THROW_COND_DB
    (
        (aRowId < iRowsCount) && (aColId < iColumnsCount),
        GpDbExceptionCode::OUT_OF_RANGE_ROW_COL,
        [&]()
        {
            return fmt::format
            (
                "Requested column id[{}][{}] is out of range [0..{}), [0..{})",
                aRowId,
                aColId,
                iRowsCount,
                iColumnsCount
            );
        }
    );

    // Get data ptr
    GpSpanByteR dataPtr = iRowColDataVec[iRowsCount*aRowId + aColId];

    // Check data description
    THROW_COND_DB
    (
        iRowDescOpt.has_value(),
        GpDbExceptionCode::EMPTY_RS_MODEL_DESC,
        "No data description is available for the response"
    );

    // Get column desc
    const PSQL::RowDescriptionDescRS::ColumnDesc& columnDesc = iRowDescOpt.value().columns[aColId];

    // Check IOD for column
    THROW_COND_DB
    (
        columnDesc.type_oid == u_int_32(aTypeOID),
        GpDbExceptionCode::WRONG_TYPE_OID,
        [&]()
        {
            return fmt::format
            (
                "Wrong IOD type {} for column id[{}][{}]. Expected type {}",
                columnDesc.type_oid,
                aRowId,
                aColId,
                int(aTypeOID)
            );
        }
    );

    return
    {
        iRowColDataStorage.data() + reinterpret_cast<size_t>(dataPtr.Ptr()),
        dataPtr.Count()
    };
}

}// namespace GPlatform

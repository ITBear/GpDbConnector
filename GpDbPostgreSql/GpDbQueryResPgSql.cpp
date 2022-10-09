#include "GpDbQueryResPgSql.hpp"

namespace GPlatform {

template<typename T, size_t ExpectedSizeT>
T   GpDbQueryResPgSql_GetPOD
(
    const size_t        aRowId,
    const size_t        aColId,
    std::optional<T>    aOnNullValue,
    PGresult*           aPgResult
)
{
    static_assert(sizeof(T) == ExpectedSizeT);

    const int rowId = NumOps::SConvert<int>(aRowId);
    const int colId = NumOps::SConvert<int>(aColId);

    if (PQgetisnull(aPgResult, rowId, colId))
    {
        THROW_COND_GP
        (
            aOnNullValue.has_value(),
            [&](){return "Value on ["_sv + aRowId + ", "_sv + aColId + "] is NULL"_sv;}
        );

        return aOnNullValue.value();
    }

    const int len = PQgetlength(aPgResult, rowId, colId);

    THROW_COND_GP
    (
        size_t(len) == sizeof(T),
        [&](){return "Length must be "_sv + sizeof(T) + " bytes"_sv;}
    );

    T value;
    std::memcpy(&value, PQgetvalue(aPgResult, rowId, colId), size_t(len));
    return BitOps::N2H(value);
}

GpDbQueryResPgSql::GpDbQueryResPgSql (PGresult* aPgResult):
iPgResult(aPgResult)
{
    static_assert(PGRES_EMPTY_QUERY     == 0, "PGRES_EMPTY_QUERY    != 0");
    static_assert(PGRES_COMMAND_OK      == 1, "PGRES_COMMAND_OK     != 1");
    static_assert(PGRES_TUPLES_OK       == 2, "PGRES_TUPLES_OK      != 2");
    static_assert(PGRES_COPY_OUT        == 3, "PGRES_COPY_OUT       != 3");
    static_assert(PGRES_COPY_IN         == 4, "PGRES_COPY_IN        != 4");
    static_assert(PGRES_BAD_RESPONSE    == 5, "PGRES_BAD_RESPONSE   != 5");
    static_assert(PGRES_NONFATAL_ERROR  == 6, "PGRES_NONFATAL_ERROR != 6");
    static_assert(PGRES_FATAL_ERROR     == 7, "PGRES_FATAL_ERROR    != 7");
    static_assert(PGRES_COPY_BOTH       == 8, "PGRES_COPY_BOTH      != 8");
    static_assert(PGRES_SINGLE_TUPLE    == 9, "PGRES_SINGLE_TUPLE   != 9");
}

GpDbQueryResPgSql::~GpDbQueryResPgSql (void) noexcept
{
    ClearPgSql();
}

void    GpDbQueryResPgSql::Process
(
    const size_t    aMinResultRowsCount,
    PGconn*         aPgConn
)
{
    THROW_COND_GP
    (
        iPgResult != nullptr,
        [&](){return "PGresult is null: "_sv + std::string_view(PQerrorMessage(aPgConn));}
    );

    const ExecStatusType    pgResStatus = PQresultStatus(iPgResult);
    std::string_view        errMsg;

    switch (pgResStatus)
    {
        case PGRES_EMPTY_QUERY:     // empty query string was executed
        {
            if (aMinResultRowsCount > 0)
            {
                THROW_DB(GpDbExceptionCode::QUERY_RESULT_COUNT_LOW, ""_sv);
            }
        } break;
        case PGRES_COMMAND_OK:      // a query command that doesn't return anything was executed properly by the backend
        case PGRES_TUPLES_OK:       // a query command that returns tuples was executed properly by the backend, PGresult contains the result tuples
        case PGRES_SINGLE_TUPLE:    // single tuple from larger resultset
        case PGRES_NONFATAL_ERROR:  // notice or warning message
        {
            //OK
        } break;
        case PGRES_COPY_OUT:        // copy Out data transfer in progress
        {
            errMsg = "copy Out data transfer in progress"_sv;
        } break;
        case PGRES_COPY_IN:         // copy In data transfer in progress
        {
            errMsg = "copy In data transfer in progress"_sv;
        } break;
        case PGRES_BAD_RESPONSE:    // an unexpected response was recv'd from the backend
        {
            errMsg = "an unexpected response was recv'd from the backend"_sv;
        } break;
        case PGRES_FATAL_ERROR:     // query failed
        {
            errMsg = "query failed"_sv;
        } break;
        case PGRES_COPY_BOTH:       // Copy In/Out data transfer in progress
        {
            errMsg = "Copy In/Out data transfer in progress"_sv;
        } break;
        case PGRES_PIPELINE_SYNC:
        {
            errMsg = "single tuple from larger resultset pipeline synchronization point"_sv;
        } break;
        case PGRES_PIPELINE_ABORTED:
        {
            errMsg = "Command didn't run because of an abort earlier in a pipeline"_sv;
        } break;
        default:
        {
            errMsg = "Unknown error"_sv;
        }
    }

    if (errMsg.length() > 0)
    {
        Clear();
        ThrowDbEx(errMsg, aPgConn);
    }

    if (aMinResultRowsCount > 0)
    {
        if (RowsCount() < aMinResultRowsCount)
        {
            THROW_DB(GpDbExceptionCode::QUERY_RESULT_COUNT_LOW, "RowsCount() < aMinResultRowsCount"_sv);
        }
    }
}

void    GpDbQueryResPgSql::Clear (void)
{
    ClearPgSql();
}

GpDbQueryRes::StateTE   GpDbQueryResPgSql::State (void) const
{
    const ExecStatusType pgStatus = PQresultStatus(iPgResult);
    return GpDbQueryRes::StateTE(int(pgStatus));
}

size_t  GpDbQueryResPgSql::RowsCount (void) const
{
    int rowsCount = PQntuples(iPgResult);

    if (rowsCount < 0)
    {
        rowsCount = 0;
    }

    return NumOps::SConvert<size_t>(rowsCount);
}

size_t  GpDbQueryResPgSql::ColumnsCount (void) const
{
    int columnsCount= PQnfields(iPgResult);

    if (columnsCount < 0)
    {
        columnsCount = 0;
    }

    return NumOps::SConvert<size_t>(columnsCount);
}

s_int_16    GpDbQueryResPgSql::GetInt16
(
    const size_t            aRowId,
    const size_t            aColId,
    std::optional<s_int_16> aOnNullValue
) const
{
    return GpDbQueryResPgSql_GetPOD<s_int_16, 2>(aRowId, aColId, aOnNullValue, iPgResult);
}

std::vector<s_int_16>   GpDbQueryResPgSql::GetInt16Array1D
(
    const size_t                            aRowId,
    const size_t                            aColId,
    std::optional<std::vector<s_int_16>>    aOnNullValue
) const
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

s_int_32    GpDbQueryResPgSql::GetInt32
(
    const size_t            aRowId,
    const size_t            aColId,
    std::optional<s_int_32> aOnNullValue
) const
{
    return GpDbQueryResPgSql_GetPOD<s_int_32, 4>(aRowId, aColId, aOnNullValue, iPgResult);
}

std::vector<s_int_32>   GpDbQueryResPgSql::GetInt32Array1D
(
    const size_t                        aRowId,
    const size_t                        aColId,
    std::optional<std::vector<s_int_32>>    aOnNullValue
) const
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

s_int_64    GpDbQueryResPgSql::GetInt64
(
    const size_t            aRowId,
    const size_t            aColId,
    std::optional<s_int_64> aOnNullValue
) const
{
    return GpDbQueryResPgSql_GetPOD<s_int_64, 8>(aRowId, aColId, aOnNullValue, iPgResult);
}

std::vector<s_int_64>   GpDbQueryResPgSql::GetInt64Array1D
(
    const size_t                        aRowId,
    const size_t                        aColId,
    std::optional<std::vector<s_int_64>>    aOnNullValue
) const
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

double  GpDbQueryResPgSql::GetDouble
(
    const size_t            aRowId,
    const size_t            aColId,
    std::optional<double>   aOnNullValue
) const
{
    return GpDbQueryResPgSql_GetPOD<double, 8>(aRowId, aColId, aOnNullValue, iPgResult);
}

std::vector<double> GpDbQueryResPgSql::GetDoubleArray1D
(
    const size_t                    aRowId,
    const size_t                    aColId,
    std::optional<std::vector<double>>  aOnNullValue
) const
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

float   GpDbQueryResPgSql::GetFloat
(
    const size_t            aRowId,
    const size_t            aColId,
    std::optional<float>    aOnNullValue
) const
{
    return GpDbQueryResPgSql_GetPOD<float, 4>(aRowId, aColId, aOnNullValue, iPgResult);
}

std::vector<float>  GpDbQueryResPgSql::GetFloatArray1D
(
    const size_t                    aRowId,
    const size_t                    aColId,
    std::optional<std::vector<float>>   aOnNullValue
) const
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

std::string_view    GpDbQueryResPgSql::GetStr
(
    const size_t                    aRowId,
    const size_t                    aColId,
    std::optional<std::string_view> aOnNullValue
) const
{
    const int rowId = NumOps::SConvert<int>(aRowId);
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

    const char*     strPtr  = PQgetvalue(iPgResult, rowId, colId);
    const size_t    strLen  = NumOps::SConvert<size_t>(PQgetlength(iPgResult, rowId, colId));

    return std::string_view(strPtr, strLen);
}

std::vector<std::string_view>   GpDbQueryResPgSql::GetStrArray1D
(
    const size_t                            aRowId,
    const size_t                            aColId,
    std::optional<std::vector<std::string>> aOnNullValue
) const
{
    //TODO: implement
    //THROW_GP_NOT_IMPLEMENTED();
    return {};
}

GpSpanPtrCharRW GpDbQueryResPgSql::GetStrRW
(
    const size_t                    aRowId,
    const size_t                    aColId,
    std::optional<GpSpanPtrCharRW>  aOnNullValue
)
{
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

    return GpSpanPtrCharRW(const_cast<char*>(str.data()), str.size());
}

std::vector<GpSpanPtrCharRW>    GpDbQueryResPgSql::GetStrRWArray1D
(
    const size_t                                aRowId,
    const size_t                                aColId,
    std::optional<std::vector<GpSpanPtrCharRW>> aOnNullValue
)
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

std::string_view    GpDbQueryResPgSql::GetJson
(
    const size_t                    aRowId,
    const size_t                    aColId,
    std::optional<std::string_view> aOnNullValue
) const
{
    std::string_view str = GetStr(aRowId, aColId, aOnNullValue);

    THROW_COND_GP
    (
        str.length() >= 3,
        "json data length must be >= 3 bytes"_sv
    );

    THROW_COND_GP
    (
        std::bit_cast<u_int_8>(str.data()[0]) == 1,
        "Wrong pgJson format version"_sv
    );

    return str.substr(1, str.length() - 1);
}

std::vector<std::string_view>   GpDbQueryResPgSql::GetJsonArray1D
(
    const size_t                            aRowId,
    const size_t                            aColId,
    std::optional<std::vector<std::string>> aOnNullValue
) const
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

GpSpanPtrCharRW GpDbQueryResPgSql::GetJsonRW
(
    const size_t                    aRowId,
    const size_t                    aColId,
    std::optional<GpSpanPtrCharRW>  aOnNullValue
)
{
    std::optional<std::string_view> defaultValue;

    if (aOnNullValue.has_value())
    {
        defaultValue = aOnNullValue.value().AsStringView();
    }

    std::string_view str = std::as_const(*this).GetJson
    (
        aRowId,
        aColId,
        defaultValue
    );

    return GpSpanPtrCharRW(const_cast<char*>(str.data()), str.size());
}

std::vector<GpSpanPtrCharRW>    GpDbQueryResPgSql::GetJsonRWArray1D
(
    const size_t                                aRowId,
    const size_t                                aColId,
    std::optional<std::vector<GpSpanPtrCharRW>> aOnNullValue
)
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

GpUUID  GpDbQueryResPgSql::GetUuid
(
    const size_t            aRowId,
    const size_t            aColId,
    std::optional<GpUUID>   aOnNullValue
) const
{
    const int rowId = NumOps::SConvert<int>(aRowId);
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
    std::memcpy(uuid.Data().data(), dataPtr, sizeof(GpUUID::DataT));

    return uuid;
}

std::vector<GpUUID> GpDbQueryResPgSql::GetUuidArray1D
(
    const size_t                    aRowId,
    const size_t                    aColId,
    std::optional<std::vector<GpUUID>>  aOnNullValue
) const
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

GpSpanPtrByteR  GpDbQueryResPgSql::GetBlob
(
    const size_t                    aRowId,
    const size_t                    aColId,
    std::optional<GpSpanPtrByteR>   aOnNullValue
) const
{
    const int rowId = NumOps::SConvert<int>(aRowId);
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

    return GpSpanPtrCharR(dataPtr, dataSize);
}

std::vector<GpSpanPtrByteR> GpDbQueryResPgSql::GetBlobArray1D
(
    const size_t                            aRowId,
    const size_t                            aColId,
    std::optional<std::vector<GpSpanPtrByteR>>  aOnNullValue
) const
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

bool    GpDbQueryResPgSql::GetBoolean
(
    const size_t        aRowId,
    const size_t        aColId,
    std::optional<bool> aOnNullValue
) const
{
    const int rowId = NumOps::SConvert<int>(aRowId);
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

    std::string_view str(dataPtr, dataSize);
    const char v = str.at(0);

    return     (v == 0x01)
            || (v == 't')
            || (v == 'T')
            || (v == 'y')
            || (v == 'Y');
}

std::vector<bool>   GpDbQueryResPgSql::GetBooleanArray1D
(
    const size_t                    aRowId,
    const size_t                    aColId,
    std::optional<std::vector<bool>>    aOnNullValue
) const
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

void    GpDbQueryResPgSql::ClearPgSql (void) noexcept
{
    if (iPgResult != nullptr)
    {
        PQclear(iPgResult);
        iPgResult = nullptr;
    }
}

void    GpDbQueryResPgSql::ThrowDbEx
(
    std::string_view    aMsg,
    PGconn*             aPgConn
)
{
    std::string_view            message = std::string_view(PQerrorMessage(aPgConn));
    GpDbExceptionCode::EnumT    code    = GpDbExceptionCode::QUERY_ERROR;

    if (message.find("duplicate key"_sv) != std::string_view::npos)
    {
        code = GpDbExceptionCode::QUERY_DUPLICATE_KEY;
    }

    THROW_DB
    (
        code,
        aMsg + ": "_sv + message
    );
}

}//namespace GPlatform

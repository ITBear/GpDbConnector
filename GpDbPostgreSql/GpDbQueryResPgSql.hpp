#pragma once

#include <GpDbConnector/GpDbPostgreSql/GpDbPostgreSql_global.hpp>
#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlProtocolMessages.hpp>
#include <GpDbConnector/GpDbClient/Query/GpDbQueryRes.hpp>

namespace GPlatform {

class GpDbQueryResPgSql final: public GpDbQueryRes
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbQueryResPgSql)
    CLASS_DD(GpDbQueryResPgSql)

    using RowColDataVecT    = boost::container::small_vector<GpSpanByteR, 16*4>;

public:
                                            GpDbQueryResPgSql   (void);
    virtual                                 ~GpDbQueryResPgSql  (void) noexcept override final;

    void                                    AddDataRow          (const PSQL::RowDescriptionDescRS&  aRowDesc,
                                                                 const PSQL::DataRowDescRS&         aRowData);

    [[nodiscard]]
    virtual size_t                          RowsCount           (void) const override final;

    [[nodiscard]]
    virtual size_t                          ColumnsCount        (void) const override final;

    [[nodiscard]]
    virtual s_int_16                        GetInt16            (size_t                     aRowId,
                                                                 size_t                     aColId,
                                                                 std::optional<s_int_16>    aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<s_int_16>           GetInt16Array1D     (size_t                                 aRowId,
                                                                 size_t                                 aColId,
                                                                 std::optional<std::vector<s_int_16>>   aOnNullValue) const override final;

    [[nodiscard]]
    virtual s_int_32                        GetInt32            (size_t                     aRowId,
                                                                 size_t                     aColId,
                                                                 std::optional<s_int_32>    aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<s_int_32>           GetInt32Array1D     (size_t                                 aRowId,
                                                                 size_t                                 aColId,
                                                                 std::optional<std::vector<s_int_32>>   aOnNullValue) const override final;

    [[nodiscard]]
    virtual s_int_64                        GetInt64            (size_t                     aRowId,
                                                                 size_t                     aColId,
                                                                 std::optional<s_int_64>    aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<s_int_64>           GetInt64Array1D     (size_t                                 aRowId,
                                                                 size_t                                 aColId,
                                                                 std::optional<std::vector<s_int_64>>   aOnNullValue) const override final;

    [[nodiscard]]
    virtual double                          GetDouble           (size_t                 aRowId,
                                                                 size_t                 aColId,
                                                                 std::optional<double>  aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<double>             GetDoubleArray1D    (size_t                             aRowId,
                                                                 size_t                             aColId,
                                                                 std::optional<std::vector<double>> aOnNullValue) const override final;

    [[nodiscard]]
    virtual float                           GetFloat            (size_t                 aRowId,
                                                                 size_t                 aColId,
                                                                 std::optional<float>   aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<float>              GetFloatArray1D     (size_t                             aRowId,
                                                                 size_t                             aColId,
                                                                 std::optional<std::vector<float>>  aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::string_view                GetStr              (size_t                             aRowId,
                                                                 size_t                             aColId,
                                                                 std::optional<std::string_view>    aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<std::string_view>   GetStrArray1D       (size_t                                         aRowId,
                                                                 size_t                                         aColId,
                                                                 std::optional<std::vector<std::string_view>>   aOnNullValue) const override final;

    [[nodiscard]]
    virtual GpSpanCharRW                    GetStrRW            (size_t                         aRowId,
                                                                 size_t                         aColId,
                                                                 std::optional<GpSpanCharRW>    aOnNullValue) override final;

     [[nodiscard]]
    virtual std::vector<GpSpanCharRW>       GetStrRWArray1D     (size_t                                     aRowId,
                                                                 size_t                                     aColId,
                                                                 std::optional<std::vector<GpSpanCharRW>>   aOnNullValue) override final;

    [[nodiscard]]
    virtual std::string_view                GetJson             (size_t                             aRowId,
                                                                 size_t                             aColId,
                                                                 std::optional<std::string_view>    aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<std::string_view>   GetJsonArray1D      (size_t                                         aRowId,
                                                                 size_t                                         aColId,
                                                                 std::optional<std::vector<std::string_view>>   aOnNullValue) const override final;

    [[nodiscard]]
    virtual GpSpanCharRW                    GetJsonRW           (size_t                         aRowId,
                                                                 size_t                         aColId,
                                                                 std::optional<GpSpanCharRW>    aOnNullValue) override final;

     [[nodiscard]]
    virtual std::vector<GpSpanCharRW>       GetJsonRWArray1D    (size_t                                     aRowId,
                                                                 size_t                                     aColId,
                                                                 std::optional<std::vector<GpSpanCharRW>>   aOnNullValue) override final;

    [[nodiscard]]
    virtual GpUUID                          GetUuid             (size_t                 aRowId,
                                                                 size_t                 aColId,
                                                                 std::optional<GpUUID>  aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<GpUUID>             GetUuidArray1D      (size_t                             aRowId,
                                                                 size_t                             aColId,
                                                                 std::optional<std::vector<GpUUID>> aOnNullValue) const override final;

    [[nodiscard]]
    virtual GpSpanByteR                     GetBlob             (size_t                     aRowId,
                                                                 size_t                     aColId,
                                                                 std::optional<GpSpanByteR> aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<GpSpanByteR>        GetBlobArray1D      (size_t                                     aRowId,
                                                                 size_t                                     aColId,
                                                                 std::optional<std::vector<GpSpanByteR>>    aOnNullValue) const override final;

    [[nodiscard]]
    virtual bool                            GetBoolean          (size_t                 aRowId,
                                                                 size_t                 aColId,
                                                                 std::optional<bool>    aOnNullValue) const override final;

private:
    GpSpanByteR                             RowColDataPtr       (size_t         aRowId,
                                                                 size_t         aColId,
                                                                 PSQL::TypeOID  aTypeOID) const;

private:
    std::optional<PSQL::RowDescriptionDescRS>   iRowDescOpt;
    RowColDataVecT                              iRowColDataVec;
    GpBytesArray                                iRowColDataStorage;
    size_t                                      iRowsCount      = 0;
    size_t                                      iColumnsCount   = 0;
};

}// namespace GPlatform

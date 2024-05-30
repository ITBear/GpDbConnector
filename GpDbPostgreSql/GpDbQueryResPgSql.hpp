#pragma once

#include "GpDbPostgreSql_global.hpp"
#include "../GpDbClient/Query/GpDbQueryRes.hpp"
#include <postgresql/libpq-fe.h>

namespace GPlatform {

class GpDbQueryResPgSql final: public GpDbQueryRes
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbQueryResPgSql)
    CLASS_DD(GpDbQueryResPgSql)

public:
                                            GpDbQueryResPgSql   (PGresult* aPgResult);
    virtual                                 ~GpDbQueryResPgSql  (void) noexcept override final;

    void                                    Process             (const size_t   aMinResultRowsCount,
                                                                 PGconn*        aPgConn);

    virtual void                            Clear               (void) override final;

    [[nodiscard]]
    virtual StateTE                         State               (void) const override final;

    [[nodiscard]]
    virtual size_t                          RowsCount           (void) const override final;

    [[nodiscard]]
    virtual size_t                          ColumnsCount        (void) const override final;

    [[nodiscard]]
    virtual s_int_16                        GetInt16            (const size_t               aRowId,
                                                                 const size_t               aColId,
                                                                 std::optional<s_int_16>    aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<s_int_16>           GetInt16Array1D     (const size_t                           aRowId,
                                                                 const size_t                           aColId,
                                                                 std::optional<std::vector<s_int_16>>   aOnNullValue) const override final;

    [[nodiscard]]
    virtual s_int_32                        GetInt32            (const size_t               aRowId,
                                                                 const size_t               aColId,
                                                                 std::optional<s_int_32>    aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<s_int_32>           GetInt32Array1D     (const size_t                           aRowId,
                                                                 const size_t                           aColId,
                                                                 std::optional<std::vector<s_int_32>>   aOnNullValue) const override final;

    [[nodiscard]]
    virtual s_int_64                        GetInt64            (const size_t               aRowId,
                                                                 const size_t               aColId,
                                                                 std::optional<s_int_64>    aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<s_int_64>           GetInt64Array1D     (const size_t                           aRowId,
                                                                 const size_t                           aColId,
                                                                 std::optional<std::vector<s_int_64>>   aOnNullValue) const override final;

    [[nodiscard]]
    virtual double                          GetDouble           (const size_t           aRowId,
                                                                 const size_t           aColId,
                                                                 std::optional<double>  aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<double>             GetDoubleArray1D    (const size_t                       aRowId,
                                                                 const size_t                       aColId,
                                                                 std::optional<std::vector<double>> aOnNullValue) const override final;

    [[nodiscard]]
    virtual float                           GetFloat            (const size_t           aRowId,
                                                                 const size_t           aColId,
                                                                 std::optional<float>   aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<float>              GetFloatArray1D     (const size_t                       aRowId,
                                                                 const size_t                       aColId,
                                                                 std::optional<std::vector<float>>  aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::string_view                GetStr              (const size_t                       aRowId,
                                                                 const size_t                       aColId,
                                                                 std::optional<std::string_view>    aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<std::string_view>   GetStrArray1D       (const size_t                                   aRowId,
                                                                 const size_t                                   aColId,
                                                                 std::optional<std::vector<std::string_view>>   aOnNullValue) const override final;

    [[nodiscard]]
    virtual GpSpanCharRW                    GetStrRW            (const size_t                   aRowId,
                                                                 const size_t                   aColId,
                                                                 std::optional<GpSpanCharRW>    aOnNullValue) override final;

     [[nodiscard]]
    virtual std::vector<GpSpanCharRW>       GetStrRWArray1D     (const size_t                               aRowId,
                                                                 const size_t                               aColId,
                                                                 std::optional<std::vector<GpSpanCharRW>>   aOnNullValue) override final;

    [[nodiscard]]
    virtual std::string_view                GetJson             (const size_t                       aRowId,
                                                                 const size_t                       aColId,
                                                                 std::optional<std::string_view>    aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<std::string_view>   GetJsonArray1D      (const size_t                                   aRowId,
                                                                 const size_t                                   aColId,
                                                                 std::optional<std::vector<std::string_view>>   aOnNullValue) const override final;

    [[nodiscard]]
    virtual GpSpanCharRW                    GetJsonRW           (const size_t                   aRowId,
                                                                 const size_t                   aColId,
                                                                 std::optional<GpSpanCharRW>    aOnNullValue) override final;

     [[nodiscard]]
    virtual std::vector<GpSpanCharRW>       GetJsonRWArray1D    (const size_t                               aRowId,
                                                                 const size_t                               aColId,
                                                                 std::optional<std::vector<GpSpanCharRW>>   aOnNullValue) override final;

    [[nodiscard]]
    virtual GpUUID                          GetUuid             (const size_t           aRowId,
                                                                 const size_t           aColId,
                                                                 std::optional<GpUUID>  aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<GpUUID>             GetUuidArray1D      (const size_t                       aRowId,
                                                                 const size_t                       aColId,
                                                                 std::optional<std::vector<GpUUID>> aOnNullValue) const override final;

    [[nodiscard]]
    virtual GpSpanByteR                     GetBlob             (const size_t               aRowId,
                                                                 const size_t               aColId,
                                                                 std::optional<GpSpanByteR> aOnNullValue) const override final;

    [[nodiscard]]
    virtual std::vector<GpSpanByteR>        GetBlobArray1D      (const size_t                               aRowId,
                                                                 const size_t                               aColId,
                                                                 std::optional<std::vector<GpSpanByteR>>    aOnNullValue) const override final;

    [[nodiscard]]
    virtual bool                            GetBoolean          (const size_t           aRowId,
                                                                 const size_t           aColId,
                                                                 std::optional<bool>    aOnNullValue) const override final;

private:
    void                                    ClearPgSql          (void) noexcept;
    void                                    ThrowDbEx           (std::string_view   aMsg,
                                                                 PGconn*            aPgConn);

private:
    PGresult*                               iPgResult = nullptr;
};

}// namespace GPlatform

#pragma once

#include "GpDbQueryResState.hpp"
#include "../../../GpCore2/GpReflection/GpReflectObject.hpp"

namespace GPlatform {

class GP_DB_CLIENT_API GpDbQueryRes
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbQueryRes)
    CLASS_DD(GpDbQueryRes)

    using StateTE = GpDbQueryResState::EnumT;

public:
                                            GpDbQueryRes        (void) noexcept {}
    virtual                                 ~GpDbQueryRes       (void) noexcept {}

    virtual void                            Clear               (void) = 0;

    [[nodiscard]]
    virtual StateTE                         State               (void) const = 0;

    [[nodiscard]]
    virtual size_t                          RowsCount           (void) const = 0;

    [[nodiscard]]
    virtual size_t                          ColumnsCount        (void) const = 0;

    [[nodiscard]]
    virtual s_int_16                        GetInt16            (const size_t               aRowId,
                                                                 const size_t               aColId,
                                                                 std::optional<s_int_16>    aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<s_int_16>           GetInt16Array1D     (const size_t                           aRowId,
                                                                 const size_t                           aColId,
                                                                 std::optional<std::vector<s_int_16>>   aOnNullValue) const = 0;

    [[nodiscard]]
    virtual s_int_32                        GetInt32            (const size_t               aRowId,
                                                                 const size_t               aColId,
                                                                 std::optional<s_int_32>    aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<s_int_32>           GetInt32Array1D     (const size_t                           aRowId,
                                                                 const size_t                           aColId,
                                                                 std::optional<std::vector<s_int_32>>   aOnNullValue) const = 0;

    [[nodiscard]]
    virtual s_int_64                        GetInt64            (const size_t               aRowId,
                                                                 const size_t               aColId,
                                                                 std::optional<s_int_64>    aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<s_int_64>           GetInt64Array1D     (const size_t                           aRowId,
                                                                 const size_t                           aColId,
                                                                 std::optional<std::vector<s_int_64>>   aOnNullValue) const = 0;

    [[nodiscard]]
    virtual double                          GetDouble           (const size_t           aRowId,
                                                                 const size_t           aColId,
                                                                 std::optional<double>  aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<double>             GetDoubleArray1D    (const size_t                       aRowId,
                                                                 const size_t                       aColId,
                                                                 std::optional<std::vector<double>> aOnNullValue) const = 0;

    [[nodiscard]]
    virtual float                           GetFloat            (const size_t           aRowId,
                                                                 const size_t           aColId,
                                                                 std::optional<float>   aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<float>              GetFloatArray1D     (const size_t                       aRowId,
                                                                 const size_t                       aColId,
                                                                 std::optional<std::vector<float>>  aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::u8string_view              GetStr              (const size_t                       aRowId,
                                                                 const size_t                       aColId,
                                                                 std::optional<std::u8string_view>  aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<std::u8string_view> GetStrArray1D       (const size_t                               aRowId,
                                                                 const size_t                               aColId,
                                                                 std::optional<std::vector<std::u8string>>  aOnNullValue) const = 0;

    [[nodiscard]]
    virtual GpSpanPtrCharRW                 GetStrRW            (const size_t                   aRowId,
                                                                 const size_t                   aColId,
                                                                 std::optional<GpSpanPtrCharRW> aOnNullValue) = 0;

    [[nodiscard]]
    virtual std::vector<GpSpanPtrCharRW>    GetStrRWArray1D     (const size_t                                   aRowId,
                                                                 const size_t                                   aColId,
                                                                 std::optional<std::vector<GpSpanPtrCharRW>>    aOnNullValue) = 0;

    [[nodiscard]]
    virtual std::u8string_view              GetJson             (const size_t                       aRowId,
                                                                 const size_t                       aColId,
                                                                 std::optional<std::u8string_view>  aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<std::u8string_view> GetJsonArray1D      (const size_t                               aRowId,
                                                                 const size_t                               aColId,
                                                                 std::optional<std::vector<std::u8string>>  aOnNullValue) const = 0;

    [[nodiscard]]
    virtual GpSpanPtrCharRW                 GetJsonRW           (const size_t                   aRowId,
                                                                 const size_t                   aColId,
                                                                 std::optional<GpSpanPtrCharRW> aOnNullValue) = 0;

    [[nodiscard]]
    virtual std::vector<GpSpanPtrCharRW>    GetJsonRWArray1D    (const size_t                                   aRowId,
                                                                 const size_t                                   aColId,
                                                                 std::optional<std::vector<GpSpanPtrCharRW>>    aOnNullValue) = 0;

    [[nodiscard]]
    virtual GpUUID                          GetUuid             (const size_t           aRowId,
                                                                 const size_t           aColId,
                                                                 std::optional<GpUUID>  aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<GpUUID>             GetUuidArray1D      (const size_t                       aRowId,
                                                                 const size_t                       aColId,
                                                                 std::optional<std::vector<GpUUID>> aOnNullValue) const = 0;

    [[nodiscard]]
    virtual GpSpanPtrByteR                  GetBlob             (const size_t                   aRowId,
                                                                 const size_t                   aColId,
                                                                 std::optional<GpSpanPtrByteR>  aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<GpSpanPtrByteR>     GetBlobArray1D      (const size_t                               aRowId,
                                                                 const size_t                               aColId,
                                                                 std::optional<std::vector<GpSpanPtrByteR>> aOnNullValue) const = 0;

    [[nodiscard]]
    virtual bool                            GetBoolean          (const size_t           aRowId,
                                                                 const size_t           aColId,
                                                                 std::optional<bool>    aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<bool>               GetBooleanArray1D   (const size_t                       aRowId,
                                                                 const size_t                       aColId,
                                                                 std::optional<std::vector<bool>>   aOnNullValue) const = 0;


    template<typename T>
    [[nodiscard]] typename T::EnumT         GetEnum             (const size_t                       aRowId,
                                                                 const size_t                       aColId,
                                                                 std::optional<typename T::EnumT>   aOnNullValue) const;

    [[nodiscard]]
    GpReflectObject::SP                     ColToObject         (const size_t   aRowId,
                                                                 const size_t   aColId) const;

    [[nodiscard]]
    GpReflectObject::C::Vec::SP             ColToObjectArray1D  (const size_t   aRowId,
                                                                 const size_t   aColId) const;

    [[nodiscard]]
    GpReflectObject::SP                     ColToObject         (const size_t           aRowId,
                                                                 const size_t           aColId,
                                                                 const GpReflectModel&  aModel) const;

    template<typename T>
    [[nodiscard]] typename T::SP            ColToObjectAs       (const size_t           aRowId,
                                                                 const size_t           aColId) const;

    [[nodiscard]]
    GpReflectObject::C::Vec::SP             ColToObjectArray1D  (const size_t           aRowId,
                                                                 const size_t           aColId,
                                                                 const GpReflectModel&  aModel) const;

    [[nodiscard]]
    GpReflectObject::SP                     RowToObject         (const size_t           aRowId,
                                                                 const size_t           aColStartId,
                                                                 const GpReflectModel&  aModel) const;

    [[nodiscard]]
    GpReflectObject::C::Vec::SP             RowsToObject        (const size_t           aColStartId,
                                                                 const GpReflectModel&  aModel) const;

    template<typename T>
    [[nodiscard]] typename T::SP            RowToObjectAs       (const size_t aRowId,
                                                                 const size_t aColStartId) const;

    template<typename T>
    [[nodiscard]] typename T::C::Vec::SP    RowsToObjectAs      (const size_t aColStartId) const;

private:
    void                                    _RowToObjectProp    (void*                  aDataPtr,
                                                                 const GpReflectProp&   aProp,
                                                                 const size_t           aRowId,
                                                                 const size_t           aColId) const;

    void                                    _RowToObjectPropVec (void*                  aDataPtr,
                                                                 const GpReflectProp&   aProp,
                                                                 const size_t           aRowId,
                                                                 const size_t           aColId) const;

    void                                    _RowToObjectPropMap (void*                  aDataPtr,
                                                                 const GpReflectProp&   aProp,
                                                                 const size_t           aRowId,
                                                                 const size_t           aColId) const;

    template<typename ToT, typename  FromT>
    static std::vector<ToT>                 _SConvertArrayNum   (const std::vector<FromT>& aVec);

    template<typename ToT, typename  FromT>
    static std::vector<ToT>                 _SConvertArrayBytes (std::vector<FromT>& aVec);
};

template<typename T>
[[nodiscard]] typename T::EnumT GpDbQueryRes::GetEnum
(
    const size_t                        aRowId,
    const size_t                        aColId,
    std::optional<typename T::EnumT>    aOnNullValue
) const
{
    std::u8string_view strVal = GetStr(aRowId, aColId, {});

    if (strVal.length() == 0)
    {
        THROW_COND_GP
        (
            aOnNullValue.has_value(),
            [&](){return u8"Value on ["_sv + aRowId + u8", "_sv + aColId + u8"] is empty"_sv;}
        );
        return aOnNullValue.value();
    }

    return T::SFromString(strVal);
}

template<typename T>
[[nodiscard]] typename T::SP    GpDbQueryRes::ColToObjectAs
(
    const size_t    aRowId,
    const size_t    aColId
) const
{
    return ColToObject
    (
        aRowId,
        aColId,
        T::SReflectModel()
    ).template CastAs<typename T::SP>();
}

template<typename T>
[[nodiscard]] typename T::SP    GpDbQueryRes::RowToObjectAs
(
    const size_t aRowId,
    const size_t aColStartId
) const
{
    return RowToObject
    (
        aRowId,
        aColStartId,
        T::SReflectModel()
    ).template CastAs<typename T::SP>();
}

template<typename T>
[[nodiscard]] typename T::C::Vec::SP    GpDbQueryRes::RowsToObjectAs (const size_t aColStartId) const
{
    const size_t            rowsCount = RowsCount();
    typename T::C::Vec::SP  res;

    res.reserve(rowsCount);

    for (size_t rowId = 0; rowId < rowsCount; rowId++)
    {
        res.emplace_back
        (
            RowToObjectAs<T>
            (
                rowId,
                aColStartId
            )
        );
    }

    return res;
}

template<typename TO, typename  FROM>
std::vector<TO> GpDbQueryRes::_SConvertArrayNum (const std::vector<FROM>& aVec)
{
    const size_t size = aVec.size();
    std::vector<TO> res(size);

    for (size_t id = 0; id < size; ++id)
    {
        if constexpr (GpHasTag_GpUnit<TO>())
        {
            res[id] = TO::SMake(aVec[id]);
        } else
        {
            res[id] = NumOps::SConvert<TO>(aVec[id]);
        }
    }

    return res;
}

template<typename TO, typename  FROM>
std::vector<TO> GpDbQueryRes::_SConvertArrayBytes (std::vector<FROM>& aVec)
{
    const size_t size = aVec.size();
    std::vector<TO> res(size);

    for (size_t id = 0; id < size; ++id)
    {
        res[id] = static_cast<TO>(aVec[id]);
    }

    return res;
}

}//namespace GPlatform

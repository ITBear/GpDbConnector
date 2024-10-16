#pragma once

#include <GpDbConnector/GpDbClient/GpDbClient_global.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>

namespace GPlatform {

class GP_DB_CLIENT_API GpDbQueryRes
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbQueryRes)
    CLASS_DD(GpDbQueryRes)

public:
                                            GpDbQueryRes            (void) noexcept {}
    virtual                                 ~GpDbQueryRes           (void) noexcept {}

    [[nodiscard]]
    virtual size_t                          RowsCount               (void) const = 0;

    [[nodiscard]]
    virtual size_t                          ColumnsCount            (void) const = 0;

    [[nodiscard]]
    virtual s_int_16                        GetInt16                (size_t                     aRowId,
                                                                     size_t                     aColId,
                                                                     std::optional<s_int_16>    aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<s_int_16>           GetInt16Array1D         (size_t                                 aRowId,
                                                                     size_t                                 aColId,
                                                                     std::optional<std::vector<s_int_16>>   aOnNullValue) const = 0;

    [[nodiscard]]
    virtual s_int_32                        GetInt32                (size_t                     aRowId,
                                                                     size_t                     aColId,
                                                                     std::optional<s_int_32>    aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<s_int_32>           GetInt32Array1D         (size_t                                 aRowId,
                                                                     size_t                                 aColId,
                                                                     std::optional<std::vector<s_int_32>>   aOnNullValue) const = 0;

    [[nodiscard]]
    virtual s_int_64                        GetInt64                (size_t                     aRowId,
                                                                     size_t                     aColId,
                                                                     std::optional<s_int_64>    aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<s_int_64>           GetInt64Array1D         (size_t                                 aRowId,
                                                                     size_t                                 aColId,
                                                                     std::optional<std::vector<s_int_64>>   aOnNullValue) const = 0;

    [[nodiscard]]
    virtual double                          GetDouble               (size_t                 aRowId,
                                                                     size_t                 aColId,
                                                                     std::optional<double>  aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<double>             GetDoubleArray1D        (size_t                             aRowId,
                                                                     size_t                             aColId,
                                                                     std::optional<std::vector<double>> aOnNullValue) const = 0;

    [[nodiscard]]
    virtual float                           GetFloat                (size_t                 aRowId,
                                                                     size_t                 aColId,
                                                                     std::optional<float>   aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<float>              GetFloatArray1D         (size_t                             aRowId,
                                                                     size_t                             aColId,
                                                                     std::optional<std::vector<float>>  aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::string_view                GetStr                  (size_t                             aRowId,
                                                                     size_t                             aColId,
                                                                     std::optional<std::string_view>    aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<std::string_view>   GetStrArray1D           (size_t                                         aRowId,
                                                                     size_t                                         aColId,
                                                                     std::optional<std::vector<std::string_view>>   aOnNullValue) const = 0;

    [[nodiscard]]
    virtual GpSpanCharRW                    GetStrRW                (size_t                         aRowId,
                                                                     size_t                         aColId,
                                                                     std::optional<GpSpanCharRW>    aOnNullValue) = 0;

    [[nodiscard]]
    virtual std::vector<GpSpanCharRW>       GetStrRWArray1D         (size_t                                     aRowId,
                                                                     size_t                                     aColId,
                                                                     std::optional<std::vector<GpSpanCharRW>>   aOnNullValue) = 0;

    [[nodiscard]]
    virtual std::string_view                GetJson                 (size_t                             aRowId,
                                                                     size_t                             aColId,
                                                                     std::optional<std::string_view>    aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<std::string_view>   GetJsonArray1D          (size_t                                         aRowId,
                                                                     size_t                                         aColId,
                                                                     std::optional<std::vector<std::string_view>>   aOnNullValue) const = 0;

    [[nodiscard]]
    virtual GpSpanCharRW                    GetJsonRW               (size_t                         aRowId,
                                                                     size_t                         aColId,
                                                                     std::optional<GpSpanCharRW>    aOnNullValue) = 0;

    [[nodiscard]]
    virtual std::vector<GpSpanCharRW>       GetJsonRWArray1D        (size_t                                     aRowId,
                                                                     size_t                                     aColId,
                                                                     std::optional<std::vector<GpSpanCharRW>>   aOnNullValue) = 0;

    [[nodiscard]]
    virtual GpUUID                          GetUuid                 (size_t                 aRowId,
                                                                     size_t                 aColId,
                                                                     std::optional<GpUUID>  aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<GpUUID>             GetUuidArray1D          (size_t                             aRowId,
                                                                     size_t                             aColId,
                                                                     std::optional<std::vector<GpUUID>> aOnNullValue) const = 0;

    [[nodiscard]]
    virtual GpSpanByteR                     GetBlob                 (size_t                     aRowId,
                                                                     size_t                     aColId,
                                                                     std::optional<GpSpanByteR> aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::vector<GpSpanByteR>        GetBlobArray1D          (size_t                                     aRowId,
                                                                     size_t                                     aColId,
                                                                     std::optional<std::vector<GpSpanByteR>>    aOnNullValue) const = 0;

    [[nodiscard]]
    virtual bool                            GetBoolean              (size_t                 aRowId,
                                                                     size_t                 aColId,
                                                                     std::optional<bool>    aOnNullValue) const = 0;

    template<typename T>
    [[nodiscard]] typename T::EnumT         GetEnum                 (size_t                             aRowId,
                                                                     size_t                             aColId,
                                                                     std::optional<typename T::EnumT>   aOnNullValue) const;

    [[nodiscard]]
    GpReflectObject::SP                     ColToObject             (size_t aRowId,
                                                                     size_t aColId) const;

    [[nodiscard]]
    GpReflectObject::C::Vec::SP             ColToObjectArray1D      (size_t aRowId,
                                                                     size_t aColId) const;

    [[nodiscard]]
    GpReflectObject::SP                     ColToObject             (size_t                 aRowId,
                                                                     size_t                 aColId,
                                                                     const GpReflectModel&  aModel) const;

    template<typename T>
    [[nodiscard]] typename T::SP            ColToObjectAs           (size_t aRowId,
                                                                     size_t aColId) const;

    [[nodiscard]]
    GpReflectObject::C::Vec::SP             ColToObjectArray1D      (size_t                 aRowId,
                                                                     size_t                 aColId,
                                                                     const GpReflectModel&  aModel) const;

    [[nodiscard]]
    GpReflectObject::SP                     RowToObject             (size_t                 aRowId,
                                                                     size_t                 aColStartId,
                                                                     const GpReflectModel&  aModel) const;

    [[nodiscard]]
    GpReflectObject::C::Vec::SP             RowsToObjects           (size_t                 aColStartId,
                                                                     const GpReflectModel&  aModel) const;

    template<typename T>
    [[nodiscard]] typename T::SP            RowToObjectAs           (size_t aRowId,
                                                                     size_t aColStartId) const;

    template<typename T>
    [[nodiscard]] typename T::C::Vec::SP    RowsToObjectsAs         (size_t aColStartId) const;

private:
    void                                    _RowToObjectProp        (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp,
                                                                     size_t                 aRowId,
                                                                     size_t                 aColId) const;

    void                                    _RowToObjectPropVec     (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp,
                                                                     size_t                 aRowId,
                                                                     size_t                 aColId) const;

    void                                    _RowToObjectPropVecWrap (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp,
                                                                     size_t                 aRowId,
                                                                     size_t                 aColId) const;

    void                                    _RowToObjectPropMap     (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp,
                                                                     size_t                 aRowId,
                                                                     size_t                 aColId) const;

    template<typename ToT, typename  FromT>
    static std::vector<ToT>                 _SConvertArrayNum       (const std::vector<FromT>& aVec);

    template<typename ToT, typename  FromT>
    static std::vector<ToT>                 _SConvertArrayBytes     (std::vector<FromT>& aVec);
};

template<typename T>
[[nodiscard]] typename T::EnumT GpDbQueryRes::GetEnum
(
    const size_t                        aRowId,
    const size_t                        aColId,
    std::optional<typename T::EnumT>    aOnNullValue
) const
{
    std::string_view strVal = GetStr(aRowId, aColId, {});

    if (std::size(strVal) == 0)
    {
        THROW_COND_GP
        (
            aOnNullValue.has_value(),
            [&]()
            {
                return fmt::format
                (
                    "Value on [{}, {}] is empty",
                    aRowId,
                    aColId
                );
            }
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
[[nodiscard]] typename T::C::Vec::SP    GpDbQueryRes::RowsToObjectsAs (const size_t aColStartId) const
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
    const size_t size = std::size(aVec);
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
    const size_t size = std::size(aVec);
    std::vector<TO> res;
    res.resize(size);

    for (size_t id = 0; id < size; ++id)
    {
        const auto&     fromElement     = aVec[id];
        auto&           toElement       = res[id];
        const size_t    fromElementSize = std::size(fromElement);

        if (fromElementSize == 0)
        {
            continue;
        }

        toElement.resize(fromElementSize);
        std::memcpy(std::data(toElement), std::data(fromElement), fromElementSize);
    }

    return res;
}

}// namespace GPlatform

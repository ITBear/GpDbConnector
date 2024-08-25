#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlErrorResponseUtils.hpp>
#include <GpDbConnector/GpDbClient/GpDbException.hpp>

namespace FMT_NAMESPASE {

static const boost::container::small_flat_map<::GPlatform::PSQL::ErrorCodeId, std::string, 18> sErrorCodeIdToStr =
{
    {::GPlatform::PSQL::ErrorCodeId::SEVERITY_1,        "Severity1"},
    {::GPlatform::PSQL::ErrorCodeId::SEVERITY_2,        "Severity2"},
    {::GPlatform::PSQL::ErrorCodeId::SQLSTATE_CODE,     "Code"},
    {::GPlatform::PSQL::ErrorCodeId::MESSAGE,           "Message"},
    {::GPlatform::PSQL::ErrorCodeId::DETAIL,            "Detail"},
    {::GPlatform::PSQL::ErrorCodeId::HINT,              "Hint"},
    {::GPlatform::PSQL::ErrorCodeId::POSITION,          "Position"},
    {::GPlatform::PSQL::ErrorCodeId::INTERNAL_POSITION, "Internal position"},
    {::GPlatform::PSQL::ErrorCodeId::INTERNAL_QUERY,    "Internal query"},
    {::GPlatform::PSQL::ErrorCodeId::WHERE,             "Where"},
    {::GPlatform::PSQL::ErrorCodeId::SCHEMA_NAME,       "Schema name"},
    {::GPlatform::PSQL::ErrorCodeId::TABLE_NAME,        "Table name"},
    {::GPlatform::PSQL::ErrorCodeId::COLUMN_NAME,       "Column name"},
    {::GPlatform::PSQL::ErrorCodeId::DATA_TYPE_NAME,    "Data type name"},
    {::GPlatform::PSQL::ErrorCodeId::CONSTRAINT_NAME,   "Constraint name"},
    {::GPlatform::PSQL::ErrorCodeId::FILE,              "File"},
    {::GPlatform::PSQL::ErrorCodeId::LINE,              "Line"},
    {::GPlatform::PSQL::ErrorCodeId::ROUTINE,           "Routine"} // 18
};

template<>
struct formatter<::GPlatform::PSQL::ErrorResponseDescRS::MessagesT::value_type>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& aCtx)
    {
        return std::begin(aCtx);
    }

    template<typename FormatContext>
    auto format(const ::GPlatform::PSQL::ErrorResponseDescRS::MessagesT::value_type& aMsgElement, FormatContext& aCtx) const
    {
        const auto [errorCode, msg] = aMsgElement;

        std::string_view    errorCodeStr;
        const auto          errorCodeStrIter = sErrorCodeIdToStr.find(errorCode);

        if (errorCodeStrIter != sErrorCodeIdToStr.end())
        {
            errorCodeStr = errorCodeStrIter->second;
        } else
        {
            errorCodeStr = "Unknown";
        }

        return ::fmt::format_to
        (
            aCtx.out(),
            "    [{}]: {}",
            errorCodeStr,
            msg
        );
    }
};

template<>
struct formatter<::GPlatform::PSQL::ErrorResponseDescRS>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& aCtx)
    {
        return std::begin(aCtx);
    }

    template<typename FormatContext>
    auto format(const ::GPlatform::PSQL::ErrorResponseDescRS& aMsgDesc, FormatContext& aCtx) const
    {
        return ::fmt::format_to
        (
            aCtx.out(),
            "{}",
            ::fmt::join(aMsgDesc.messages, "\n")
        );
    }
};

}// namespace FMT_NAMESPASE


namespace GPlatform::PSQL {

void    ErrorResponseUtils::SThrow
(
    const PSQL::ErrorResponseDescRS&    aMsgDescOut,
    const SourceLocationT&              aSourceLocation
)
{
    THROW_DB
    (
        GpDbExceptionCode::REQUEST_ERROR,
        fmt::format("PostgreSQL error:\n{}", aMsgDescOut),
        aSourceLocation
    );
}

}// namespace GPlatform::PSQL

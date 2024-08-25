#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlProtocolDeserializer.hpp>
#include <GpDbConnector/GpDbClient/GpDbException.hpp>

namespace GPlatform::PSQL {

void    ProtocolDeserializer::SDeserialize
(
    GpByteReader&               aDataReader,
    PSQL::ErrorResponseDescRS&  aMsgDescOut
)
{
    aMsgDescOut.length = aDataReader.UI32();

    if (aMsgDescOut.length == 0)
    {
        [[maybe_unused]] int d = 0;
    }

    while (aDataReader.SizeLeft() > 1/*last message byte is 0*/)
    {
        const ErrorCodeId       errorCodeId = static_cast<ErrorCodeId>(aDataReader.UI8());
        const std::string_view  message     = aDataReader.NullTerminatedString();

        aMsgDescOut.messages.emplace_back(errorCodeId, message);
    }
}

void    ProtocolDeserializer::SDeserialize
(
    GpByteReader&               aDataReader,
    PSQL::AuthenticationDescRS& aMsgDescOut
)
{
    aMsgDescOut.length      = aDataReader.UI32();
    aMsgDescOut.auth_method = static_cast<AuthenticationMethod>(aDataReader.UI32());

    switch (aMsgDescOut.auth_method)
    {
        case AuthenticationMethod::AUTH_OK:
        {
            return;
        } break;
        case AuthenticationMethod::KERBEROS_V5:
        {
            THROW_DB
            (
                GpDbExceptionCode::UNSUPPORTED_FEATURE,
                "Unsupported authentication method KERBEROS_V5"_sv
            );
        } break;
        case AuthenticationMethod::CLEAR_TEXT_PASSWORD:
        {
            THROW_DB
            (
                GpDbExceptionCode::UNSUPPORTED_FEATURE,
                "Unsupported authentication method CLEAR_TEXT_PASSWORD"_sv
            );
        } break;
        case AuthenticationMethod::MD5_PASSWORD:
        {
            THROW_DB
            (
                GpDbExceptionCode::UNSUPPORTED_FEATURE,
                "Unsupported authentication method MD5_PASSWORD"_sv
            );
        } break;
        case AuthenticationMethod::GSS:
        {
            THROW_DB
            (
                GpDbExceptionCode::UNSUPPORTED_FEATURE,
                "Unsupported authentication method GSS"_sv
            );
        } break;
        case AuthenticationMethod::SSPI:
        {
            THROW_DB
            (
                GpDbExceptionCode::UNSUPPORTED_FEATURE,
                "Unsupported authentication method SSPI"_sv
            );
        } break;
        case AuthenticationMethod::SASL:
        {
            AuthenticationDescRS::AuthenticationSASL payload;

            while (aDataReader.SizeLeft() > 1/*last message byte is 0*/)
            {
                payload.names.emplace_back(aDataReader.NullTerminatedString());
            }

            aMsgDescOut.payload = std::move(payload);
        } break;
        case AuthenticationMethod::SASL_CONTINUE:
        {
            AuthenticationDescRS::AuthenticationSASLContinue payload;
            payload.server_first_message = aDataReader.Bytes(aDataReader.SizeLeft()).AsStringView();
            aMsgDescOut.payload = std::move(payload);
        } break;
        case AuthenticationMethod::SASL_FINAL:
        {
            AuthenticationDescRS::AuthenticationSASLFinal payload;
            payload.server_final_message = aDataReader.Bytes(aDataReader.SizeLeft()).AsStringView();
            aMsgDescOut.payload = std::move(payload);
        } break;
        default:
        {
            THROW_DB
            (
                GpDbExceptionCode::RESPONSE_ERROR,
                fmt::format
                (
                    "Unknown authentication method, id {}",
                    static_cast<size_t>(aMsgDescOut.auth_method)
                )
            );
        }
    }
}

}// namespace GPlatform::PSQL

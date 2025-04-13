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
            THROW
            (
                GpDbExceptionCode::UNSUPPORTED_FEATURE,
                "Unsupported authentication method KERBEROS_V5"_sv
            );
        } break;
        case AuthenticationMethod::CLEAR_TEXT_PASSWORD:
        {
            THROW
            (
                GpDbExceptionCode::UNSUPPORTED_FEATURE,
                "Unsupported authentication method CLEAR_TEXT_PASSWORD"_sv
            );
        } break;
        case AuthenticationMethod::MD5_PASSWORD:
        {
            THROW
            (
                GpDbExceptionCode::UNSUPPORTED_FEATURE,
                "Unsupported authentication method MD5_PASSWORD"_sv
            );
        } break;
        case AuthenticationMethod::GSS:
        {
            THROW
            (
                GpDbExceptionCode::UNSUPPORTED_FEATURE,
                "Unsupported authentication method GSS"_sv
            );
        } break;
        case AuthenticationMethod::SSPI:
        {
            THROW
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
            THROW
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

void    ProtocolDeserializer::SDeserialize
(
    GpByteReader&                   aDataReader,
    PSQL::ParameterStatusDescRS&    aMsgDescOut
)
{
    // length
    aMsgDescOut.length  = aDataReader.UI32();

    // name
    aMsgDescOut.name    = aDataReader.NullTerminatedString();

    // value
    aMsgDescOut.value   = aDataReader.NullTerminatedString();
}

void    ProtocolDeserializer::SDeserialize
(
    GpByteReader&               aDataReader,
    PSQL::BackendKeyDataDescRS& aMsgDescOut
)
{
    // length
    aMsgDescOut.length  = aDataReader.UI32();

    // pid
    aMsgDescOut.pid     = aDataReader.UI32();

    // secret
    aMsgDescOut.secret  = aDataReader.UI32();
}

void    ProtocolDeserializer::SDeserialize
(
    GpByteReader&               aDataReader,
    PSQL::ReadyForQueryDescRS&  aMsgDescOut
)
{
    // length
    aMsgDescOut.length = aDataReader.UI32();

    // transaction_status
    const u_int_8 transactionStatus = aDataReader.UI8();

    if (transactionStatus == static_cast<u_int_8>('I'))
    {
        aMsgDescOut.transaction_status = TransactionStatus::IDLE;
    } else if (transactionStatus == static_cast<u_int_8>('T'))
    {
        aMsgDescOut.transaction_status = TransactionStatus::TRANSACTION_BLOCK;
    } else if (transactionStatus == static_cast<u_int_8>('E'))
    {
        aMsgDescOut.transaction_status = TransactionStatus::FAILED_TRANSACTION_BLOCK;
    } else
    {
        THROW("Unknown transaction status"_sv);
    }
}

void    ProtocolDeserializer::SDeserialize
(
    GpByteReader&               aDataReader,
    PSQL::RowDescriptionDescRS& aMsgDescOut
)
{
    // length
    aMsgDescOut.length = aDataReader.UI32();

    // columns count
    const size_t columnsCount = aDataReader.UI16();

    auto& columns = aMsgDescOut.columns;
    columns.resize(columnsCount);

    for (size_t id = 0; id < columnsCount; id++)
    {
        PSQL::RowDescriptionDescRS::ColumnDesc& columnDesc = columns.data()[id];

        // name
        columnDesc.name = aDataReader.NullTerminatedString();

        // table OID
        columnDesc.table_oid = aDataReader.UI32();

        // attribute number
        columnDesc.attribute_number = aDataReader.UI16();

        // type OID
        columnDesc.type_oid = aDataReader.UI32();

        // type size
        columnDesc.type_size = aDataReader.SI16();

        // type modifier
        columnDesc.type_modifier = aDataReader.SI32();

        // format code
        columnDesc.format_code = aDataReader.UI16();
    }
}

void    ProtocolDeserializer::SDeserialize
(
    GpByteReader&           aDataReader,
    PSQL::DataRowDescRS&    aMsgDescOut
)
{
    // length
    aMsgDescOut.length = aDataReader.UI32();

    // columns count
    const size_t columnsCount = aDataReader.UI16();

    auto& columns = aMsgDescOut.columns;
    columns.resize(columnsCount);

    for (size_t id = 0; id < columnsCount; id++)
    {
        GpSpanByteRW&   columnDataPtr   = columns.data()[id];
        const ssize_t   dataSize        = aDataReader.SI32();

        if (dataSize >= 0) [[likely]]
        {
            GpSpanByteR data = aDataReader.Bytes(size_t(dataSize));

            columnDataPtr = GpSpanByteRW
            {
                const_cast<std::byte*>(data.Ptr()),
                data.Count()
            };
        }
    }
}

void    ProtocolDeserializer::SDeserialize
(
    GpByteReader&                   aDataReader,
    PSQL::CommandCompleteDescRS&    aMsgDescOut
)
{
    // length
    aMsgDescOut.length = aDataReader.UI32();

    // command
    aMsgDescOut.command = aDataReader.NullTerminatedString();
}

}// namespace GPlatform::PSQL

#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlMessageProcessor.hpp>
#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlProtocolDeserializer.hpp>
#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlProtocolSerializer.hpp>
#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlErrorResponseUtils.hpp>
#include <GpDbConnector/GpDbClient/GpDbException.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReaderStorage.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReader.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

#include <iostream>

namespace GPlatform::PSQL {

GpPsqlMessageProcessor::GpPsqlMessageProcessor
(
    std::string             aUserName,
    std::string             aPassword,
    std::string             aDatabase,
    OnReadyForRequestFnT    aReadyForRequestFn,
    OnDataRowFnT            aOnDataRowFn,
    OnCommandCompleteFnT    aOnCommandCompleteFn
) noexcept:
iUserName           {std::move(aUserName)},
iPassword           {std::move(aPassword)},
iDatabase           {std::move(aDatabase)},
iReadyForRequestFn  {std::move(aReadyForRequestFn)},
iOnDataRowFn        {std::move(aOnDataRowFn)},
iOnCommandCompleteFn{std::move(aOnCommandCompleteFn)}
{
}

GpPsqlMessageProcessor::~GpPsqlMessageProcessor (void) noexcept
{
}

size_t  GpPsqlMessageProcessor::MakeStartupMessage (GpBytesArray& aOutMessageBuffer)
{
    PSQL::StartupMessageDescRQ startupMessageDesc;

    startupMessageDesc.protocol_version         = 196608;
    startupMessageDesc.attributes["user"]       = iUserName;
    startupMessageDesc.attributes["database"]   = iDatabase;

    const size_t dataSize = PSQL::ProtocolSerializer::SSerialize(startupMessageDesc, aOutMessageBuffer);

    return dataSize;
}

size_t  GpPsqlMessageProcessor::MakeQueryMessage
(
    GpBytesArray&       aOutMessageBuffer,
    std::string_view    aQuery
)
{
    PSQL::QueryDescRQ queryMessage
    {
        .query{aQuery}
    };

    const size_t dataSize = PSQL::ProtocolSerializer::SSerialize(queryMessage, aOutMessageBuffer);

    return dataSize;
}

size_t  GpPsqlMessageProcessor::ProcessRsMessage
(
    GpSpanByteR     aRsData,
    GpBytesArray&   aOutMessageBuffer
)
{
    THROW_COND_DB
    (
        !aRsData.Empty(),
        GpDbExceptionCode::RESPONSE_ERROR,
        "Empty response"
    );

    GpByteReaderStorage dataReaderStorage{aRsData};
    GpByteReader        dataReader{dataReaderStorage};

    // Get message ID
    const PSQL::MessageRsDescId messageId = PSQL::MessageRsDescId{dataReader.UI8()};
    size_t outMessageSize = 0;

    switch (messageId)
    {
        case PSQL::MessageRsDescId::AUTHENTICATION_REQUEST:
        {
            PSQL::AuthenticationDescRS rsMsgDesc;
            PSQL::ProtocolDeserializer::SDeserialize(dataReader, rsMsgDesc);

            outMessageSize = ProcessAuthRequest(rsMsgDesc, aOutMessageBuffer);
        } break;
        case PSQL::MessageRsDescId::BACKEND_KEY_DATA:
        {
            PSQL::BackendKeyDataDescRS rsMsgDesc;
            PSQL::ProtocolDeserializer::SDeserialize(dataReader, rsMsgDesc);

            iDbPid          = rsMsgDesc.pid;
            iDbCloseSecret  = rsMsgDesc.secret;

            outMessageSize = 0;
        } break;
        case PSQL::MessageRsDescId::BIND_COMPLETE:
        {
            // TODO: implement

            LOG_ERROR
            (
                fmt::format
                (
                    "NOT IMPLEMENTED: BindComplete. Payload 0x{}",
                    StrOps::SFromBytesHex(aRsData)
                )
            );

            GpLog::S().Flush();

            THROW_GP_NOT_IMPLEMENTED();
        } break;
        case PSQL::MessageRsDescId::CLOSE_COMPLETE:
        {
            // TODO: implement

            LOG_ERROR
            (
                fmt::format
                (
                    "NOT IMPLEMENTED: CloseComplete. Payload 0x{}",
                    StrOps::SFromBytesHex(aRsData)
                )
            );

            GpLog::S().Flush();

            THROW_GP_NOT_IMPLEMENTED();
        } break;
        case PSQL::MessageRsDescId::COMMAND_COMPLETE:
        {
            PSQL::CommandCompleteDescRS commandCompleteDesc;
            PSQL::ProtocolDeserializer::SDeserialize(dataReader, commandCompleteDesc);
            iOnCommandCompleteFn(commandCompleteDesc);
        } break;
        case PSQL::MessageRsDescId::COPY_IN_RESPONSE:
        {
            // TODO: implement

            LOG_ERROR
            (
                fmt::format
                (
                    "NOT IMPLEMENTED: CopyInResponse. Payload 0x{}",
                    StrOps::SFromBytesHex(aRsData)
                )
            );

            GpLog::S().Flush();

            THROW_GP_NOT_IMPLEMENTED();
        } break;
        case PSQL::MessageRsDescId::COPY_OUT_RESPONSE:
        {
            // TODO: implement

            LOG_ERROR
            (
                fmt::format
                (
                    "NOT IMPLEMENTED: CopyOutResponse. Payload 0x{}",
                    StrOps::SFromBytesHex(aRsData)
                )
            );

            GpLog::S().Flush();

            THROW_GP_NOT_IMPLEMENTED();
        } break;
        case PSQL::MessageRsDescId::DATA_ROW:
        {
            PSQL::DataRowDescRS dataRowDesc;
            PSQL::ProtocolDeserializer::SDeserialize(dataReader, dataRowDesc);
            iOnDataRowFn(iRowDescriptionDescRS, dataRowDesc);
        } break;
        case PSQL::MessageRsDescId::ERROR_RESPONSE:
        {
            PSQL::ErrorResponseDescRS rsMsgDesc;
            PSQL::ProtocolDeserializer::SDeserialize(dataReader, rsMsgDesc);
            PSQL::ErrorResponseUtils::SThrow(rsMsgDesc);
        } break;
        case PSQL::MessageRsDescId::FUNCTION_CALL_RESPONSE:
        {
            // TODO: implement

            LOG_ERROR
            (
                fmt::format
                (
                    "NOT IMPLEMENTED: FunctionCallResponse. Payload 0x{}",
                    StrOps::SFromBytesHex(aRsData)
                )
            );

            GpLog::S().Flush();

            THROW_GP_NOT_IMPLEMENTED();
        } break;
        case PSQL::MessageRsDescId::EMPTY_DATA:
        {
            // TODO: implement

            LOG_ERROR
            (
                fmt::format
                (
                    "NOT IMPLEMENTED: NoData. Payload 0x{}",
                    StrOps::SFromBytesHex(aRsData)
                )
            );

            GpLog::S().Flush();

            THROW_GP_NOT_IMPLEMENTED();
        } break;
        case PSQL::MessageRsDescId::NOTICE_RESPONSE:
        {
            // TODO: implement

            LOG_ERROR
            (
                fmt::format
                (
                    "NOT IMPLEMENTED: NoticeResponse. Payload 0x{}",
                    StrOps::SFromBytesHex(aRsData)
                )
            );

            GpLog::S().Flush();

            THROW_GP_NOT_IMPLEMENTED();
        } break;
        case PSQL::MessageRsDescId::NOTIFICATION_RESPONSE:
        {
            // TODO: implement

            LOG_ERROR
            (
                fmt::format
                (
                    "NOT IMPLEMENTED: NotificationResponse. Payload 0x{}",
                    StrOps::SFromBytesHex(aRsData)
                )
            );

            GpLog::S().Flush();

            THROW_GP_NOT_IMPLEMENTED();
        } break;
        case PSQL::MessageRsDescId::PARAMETER_DESCRIPTION:
        {
            // TODO: implement

            LOG_ERROR
            (
                fmt::format
                (
                    "NOT IMPLEMENTED: ParameterDescription. Payload 0x{}",
                    StrOps::SFromBytesHex(aRsData)
                )
            );

            GpLog::S().Flush();

            THROW_GP_NOT_IMPLEMENTED();
        } break;
        case PSQL::MessageRsDescId::PARAMETER_STATUS:
        {
            PSQL::ParameterStatusDescRS rsMsgDesc;
            PSQL::ProtocolDeserializer::SDeserialize(dataReader, rsMsgDesc);

            iDbParameters[rsMsgDesc.name] = rsMsgDesc.value;

            outMessageSize = 0;
        } break;
        case PSQL::MessageRsDescId::PARSE_COMPLETE:
        {
            // TODO: implement

            LOG_ERROR
            (
                fmt::format
                (
                    "NOT IMPLEMENTED: ParseComplete. Payload 0x{}",
                    StrOps::SFromBytesHex(aRsData)
                )
            );

            GpLog::S().Flush();

            THROW_GP_NOT_IMPLEMENTED();
        } break;
        case PSQL::MessageRsDescId::PORTAL_SUSPENDED:
        {
            // TODO: implement

            LOG_ERROR
            (
                fmt::format
                (
                    "NOT IMPLEMENTED: PortalSuspended. Payload 0x{}",
                    StrOps::SFromBytesHex(aRsData)
                )
            );

            GpLog::S().Flush();

            THROW_GP_NOT_IMPLEMENTED();
        } break;
        case PSQL::MessageRsDescId::READY_FOR_QUERY:
        {
            PSQL::ReadyForQueryDescRS rsMsgDesc;
            PSQL::ProtocolDeserializer::SDeserialize(dataReader, rsMsgDesc);

            iTransactionStatus = rsMsgDesc.transaction_status;

            iReadyForRequestFn();

            outMessageSize = 0;
        } break;
        case PSQL::MessageRsDescId::ROW_DESCRIPTION:
        {
            iRowDescriptionDescRS.Clear();

            PSQL::ProtocolDeserializer::SDeserialize(dataReader, iRowDescriptionDescRS);
        } break;
        default:
        {
            THROW_DB
            (
                GpDbExceptionCode::RESPONSE_ERROR,
                fmt::format
                (
                    "Unknown RS message ID {}. Payload 0x{}",
                    static_cast<char>(messageId),
                    StrOps::SFromBytesHex(aRsData)
                )
            );
        }
    }

    return outMessageSize;
}

size_t  GpPsqlMessageProcessor::ProcessAuthRequest
(
    const PSQL::AuthenticationDescRS&   aRsMsgDesc,
    GpBytesArray&                       aOutMessageBuffer
)
{
    size_t outMessageSize = 0;

    switch (aRsMsgDesc.auth_method)
    {
        case PSQL::AuthenticationMethod::AUTH_OK:
        {
            //[[maybe_unused]] const auto& rsPayload = std::get<PSQL::AuthenticationDescRS::AuthenticationOk>(aRsMsgDesc.payload);
            // NOP
        } break;
        case PSQL::AuthenticationMethod::KERBEROS_V5:
        {
            THROW_DB
            (
                GpDbExceptionCode::UNSUPPORTED_FEATURE,
                "Unsupported authentication method KERBEROS_V5"_sv
            );
        } break;
        case PSQL::AuthenticationMethod::CLEAR_TEXT_PASSWORD:
        {
            THROW_DB
            (
                GpDbExceptionCode::UNSUPPORTED_FEATURE,
                "Unsupported authentication method CLEAR_TEXT_PASSWORD"_sv
            );
        } break;
        case PSQL::AuthenticationMethod::MD5_PASSWORD:
        {
            THROW_DB
            (
                GpDbExceptionCode::UNSUPPORTED_FEATURE,
                "Unsupported authentication method MD5_PASSWORD"_sv
            );
        } break;
        case PSQL::AuthenticationMethod::GSS:
        {
            THROW_DB
            (
                GpDbExceptionCode::UNSUPPORTED_FEATURE,
                "Unsupported authentication method GSS"_sv
            );
        } break;
        case PSQL::AuthenticationMethod::SSPI:
        {
            THROW_DB
            (
                GpDbExceptionCode::UNSUPPORTED_FEATURE,
                "Unsupported authentication method SSPI"_sv
            );
        } break;
        case PSQL::AuthenticationMethod::SASL:
        {
            const auto& rsPayload = std::get<PSQL::AuthenticationDescRS::AuthenticationSASL>(aRsMsgDesc.payload);

            THROW_COND_GP
            (
                std::find
                (
                    rsPayload.names.begin(),
                    rsPayload.names.end(),
                    "SCRAM-SHA-256"
                ) != rsPayload.names.end(),
                "Auth schema name expected to be SCRAM-SHA-256"_sv
            );

            iAuthCtxScramSha256SP                               = MakeSP<PSQL::GpPsqlScramSha256>(iUserName, iPassword);
            PSQL::SASLInitialResponseDescRQ clientFirstMessage  = iAuthCtxScramSha256SP->ClientFirstMessage();

            outMessageSize = PSQL::ProtocolSerializer::SSerialize(clientFirstMessage, aOutMessageBuffer);
        } break;
        case PSQL::AuthenticationMethod::SASL_CONTINUE:
        {
            const auto&                 rsPayload           = std::get<PSQL::AuthenticationDescRS::AuthenticationSASLContinue>(aRsMsgDesc.payload);
            PSQL::SASLResponseDescRQ    clientFinalMessage  = iAuthCtxScramSha256SP->ClientFinalMessage(rsPayload);

            outMessageSize = PSQL::ProtocolSerializer::SSerialize(clientFinalMessage, aOutMessageBuffer);
        } break;
        case PSQL::AuthenticationMethod::SASL_FINAL:
        {
            const auto& rsPayload = std::get<PSQL::AuthenticationDescRS::AuthenticationSASLFinal>(aRsMsgDesc.payload);

            iAuthCtxScramSha256SP->ValidateServerFinal(rsPayload);
        } break;
        default:
        {
            THROW_DB
            (
                GpDbExceptionCode::RESPONSE_ERROR,
                fmt::format
                (
                    "Unknown authentication method, id {}",
                    static_cast<size_t>(aRsMsgDesc.auth_method)
                )
            );
        }
    };

    return outMessageSize;
}

}// namespace GPlatform::PSQL

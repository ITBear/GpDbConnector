#pragma once

#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlProtocolMessages.hpp>
#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlScramSha256.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>

namespace GPlatform::PSQL {

class GpPsqlMessageProcessor
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpPsqlMessageProcessor)

    using OnReadyForRequestFnT  = std::function<void()>;
    using OnDataRowFnT          = std::function<void(const PSQL::RowDescriptionDescRS&,
                                                     const PSQL::DataRowDescRS&)>;
    using OnCommandCompleteFnT  = std::function<void(const PSQL::CommandCompleteDescRS&)>;

    using DbParametersT         = boost::container::small_flat_map<std::string, std::string, 16>;

public:
                                GpPsqlMessageProcessor  (std::string            aUserName,
                                                         std::string            aPassword,
                                                         std::string            aDatabase,
                                                         OnReadyForRequestFnT   aReadyForRequestFn,
                                                         OnDataRowFnT           aOnDataRowFn,
                                                         OnCommandCompleteFnT   aOnCommandCompleteFn) noexcept;
                                ~GpPsqlMessageProcessor (void) noexcept;

    size_t                      MakeStartupMessage      (GpBytesArray& aOutMessageBuffer/*for output messages*/);
    size_t                      MakeQueryMessage        (GpBytesArray&      aOutMessageBuffer/*for output messages*/,
                                                         std::string_view   aQuery);

    size_t                      ProcessRsMessage        (GpSpanByteR    aRsData,
                                                         GpBytesArray&  aOutMessageBuffer/*for output messages*/);

private:
    size_t                      ProcessAuthRequest      (const PSQL::AuthenticationDescRS&  aRsMsgDesc,
                                                         GpBytesArray&                      aOutMessageBuffer);

    const std::string           iUserName;
    const std::string           iPassword;
    const std::string           iDatabase;
    //PSQL::MessageRqDescId     iLastMessageRqDescId = PSQL::MessageRqDescId::NO_MESSAGE;
    PSQL::GpPsqlScramSha256::SP iAuthCtxScramSha256SP;

    // Callbacks
    OnReadyForRequestFnT        iReadyForRequestFn;
    OnDataRowFnT                iOnDataRowFn;
    OnCommandCompleteFnT        iOnCommandCompleteFn;

    //
    DbParametersT               iDbParameters;
    u_int_32                    iDbPid              = 0;    // The process ID of this backend
    u_int_32                    iDbCloseSecret      = 0;    // The secret key of this backend
    PSQL::TransactionStatus     iTransactionStatus  = PSQL::TransactionStatus::IDLE;

    // Execute RS messages
    PSQL::RowDescriptionDescRS  iRowDescriptionDescRS;
};

/*


CommandComplete

    An SQL command completed normally.
CopyInResponse

    The backend is ready to copy data from the frontend to a table; see Section 55.2.6.
CopyOutResponse

    The backend is ready to copy data from a table to the frontend; see Section 55.2.6.
RowDescription

    Indicates that rows are about to be returned in response to a SELECT, FETCH, etc. query. The contents of this message describe the column layout of the rows. This will be followed by a DataRow message for each row being returned to the frontend.
DataRow

    One of the set of rows returned by a SELECT, FETCH, etc. query.
>>>> EmptyQueryResponse

    An empty query string was recognized.
ErrorResponse

    An error has occurred.
ReadyForQuery

    Processing of the query string is complete. A separate message is sent to indicate this because the query string might contain multiple SQL commands. (CommandComplete marks the end of processing one SQL command, not the whole string.) ReadyForQuery will always be sent, whether processing terminates successfully or with an error.
NoticeResponse

    A warning message has been issued in relation to the query. Notices are in addition to other responses, i.e., the backend will continue processing the command.




 */
}// namespace GPlatform::PSQL

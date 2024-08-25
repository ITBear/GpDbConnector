#pragma once

#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlProtocolMessages.hpp>
#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlScramSha256.hpp>

namespace GPlatform::PSQL {

class GpPsqlMessageProcessor
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpPsqlMessageProcessor)

    using OnAuthOkFnT   = std::function<void()>;

public:
                                GpPsqlMessageProcessor  (std::string    aUserName,
                                                         std::string    aPassword,
                                                         std::string    aDatabase,
                                                         OnAuthOkFnT    aOnAuthOkFn) noexcept;
                                ~GpPsqlMessageProcessor (void) noexcept;


    size_t                      MakeStartupMessage      (GpBytesArray& aOutMessageBuffer/*for output messages*/);

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
    OnAuthOkFnT                 iOnAuthOkFn;
};

}// namespace GPlatform::PSQL

#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlProtocolMessages.hpp>
#include <GpCrypto/GpCryptoUtils/SASL/Scram/GpCryptoSASLScram.hpp>
#include <GpCrypto/GpCryptoCore/Utils/GpSecureStorage.hpp>

namespace GPlatform::PSQL {

// https://www.postgresql.org/docs/current/sasl-authentication.html

class GpPsqlScramSha256
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpPsqlScramSha256)
    CLASS_DD(GpPsqlScramSha256)

public:
                                GpPsqlScramSha256   (std::string    aUserName,
                                                     std::string    aPassword) noexcept;
                                ~GpPsqlScramSha256  (void) noexcept;

    SASLInitialResponseDescRQ   ClientFirstMessage  (void);
    SASLResponseDescRQ          ClientFinalMessage  (const AuthenticationDescRS::AuthenticationSASLContinue& aSASLContinueMsg);
    void                        ValidateServerFinal (const AuthenticationDescRS::AuthenticationSASLFinal& aSASLFinalMsg);

private:
    GpCryptoSASLScram           iCryptoSASLScram;
    std::string                 iUserName;
    std::string                 iPassword;
};

}// namespace GPlatform::PSQL

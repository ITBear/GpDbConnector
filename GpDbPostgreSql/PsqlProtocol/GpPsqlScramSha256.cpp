#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlScramSha256.hpp>

namespace GPlatform::PSQL {

GpPsqlScramSha256::GpPsqlScramSha256
(
    std::string aUserName,
    std::string aPassword
) noexcept:
iCryptoSASLScram{GpCryptoSASLScram::HashTypeT::SHA_256, GpCryptoSASLScram::KeyDerivationFnT::PBKDF2},
iUserName       {std::move(aUserName)},
iPassword       {std::move(aPassword)}
{
}

GpPsqlScramSha256::~GpPsqlScramSha256 (void) noexcept
{
}

SASLInitialResponseDescRQ   GpPsqlScramSha256::ClientFirstMessage (void)
{
    GpCryptoSASLScram::SmallContainerT clientFirstMessage = iCryptoSASLScram.ClientFirstMessage(iUserName);

    return PSQL::SASLInitialResponseDescRQ
    {
        .name                   = "SCRAM-SHA-256",
        .client_first_message   = GpBytesArrayUtils::SMake<std::string>(clientFirstMessage)
    };
}

SASLResponseDescRQ  GpPsqlScramSha256::ClientFinalMessage (const AuthenticationDescRS::AuthenticationSASLContinue& aSASLContinueMsg)
{
    GpCryptoSASLScram::SmallContainerT clientFinalMessage = iCryptoSASLScram.ClientFinalMessage
    (
        aSASLContinueMsg.server_first_message,
        iPassword
    );

    return SASLResponseDescRQ
    {
        .client_final_message = GpBytesArrayUtils::SMake<std::string>(clientFinalMessage)
    };
}

void    GpPsqlScramSha256::ValidateServerFinal (const AuthenticationDescRS::AuthenticationSASLFinal& aSASLFinalMsg)
{
    iCryptoSASLScram.ValidateServerFinal(aSASLFinalMsg.server_final_message);
}

}// namespace GPlatform::PSQL

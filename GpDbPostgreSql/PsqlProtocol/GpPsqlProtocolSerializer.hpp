#pragma once

#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlProtocolMessages.hpp>

namespace GPlatform::PSQL {

class ProtocolSerializer
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(ProtocolSerializer)

public:
    [[nodiscard]] static size_t SSerialize  (const StartupMessageDescRQ&        aMsgDesc,
                                             GpByteArray&                       aBufferOut);
    [[nodiscard]] static size_t SSerialize  (const SASLInitialResponseDescRQ&   aMsgDesc,
                                             GpByteArray&                       aBufferOut);
    [[nodiscard]] static size_t SSerialize  (const SASLResponseDescRQ&          aMsgDesc,
                                             GpByteArray&                       aBufferOut);
    [[nodiscard]] static size_t SSerialize  (const QueryDescRQ&                 aMsgDesc,
                                             GpByteArray&                       aBufferOut);
    [[nodiscard]] static size_t SSerialize  (const ParseDescRQ&                 aMsgDesc,
                                             GpByteArray&                       aBufferOut);
};

}// namespace GPlatform::PSQL

#pragma once

#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlProtocolMessages.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReader.hpp>

namespace GPlatform::PSQL {

class ProtocolDeserializer
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(ProtocolDeserializer)

public:
    static void     SDeserialize    (GpByteReader&              aDataReader,
                                     PSQL::ErrorResponseDescRS& aMsgDescOut);
    static void     SDeserialize    (GpByteReader&                  aDataReader,
                                     PSQL::AuthenticationDescRS&    aMsgDescOut);
    static void     SDeserialize    (GpByteReader&                  aDataReader,
                                     PSQL::ParameterStatusDescRS&   aMsgDescOut);
    static void     SDeserialize    (GpByteReader&                  aDataReader,
                                     PSQL::BackendKeyDataDescRS&    aMsgDescOut);
    static void     SDeserialize    (GpByteReader&              aDataReader,
                                     PSQL::ReadyForQueryDescRS& aMsgDescOut);
    static void     SDeserialize    (GpByteReader&                  aDataReader,
                                     PSQL::RowDescriptionDescRS&    aMsgDescOut);
    static void     SDeserialize    (GpByteReader&          aDataReader,
                                     PSQL::DataRowDescRS&   aMsgDescOut);
    static void     SDeserialize    (GpByteReader&                  aDataReader,
                                     PSQL::CommandCompleteDescRS&   aMsgDescOut);
};

}// namespace GPlatform::PSQL

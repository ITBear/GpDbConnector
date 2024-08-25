#pragma once

#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlProtocolMessages.hpp>

namespace GPlatform::PSQL {

class ErrorResponseUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(ErrorResponseUtils)

public:
    static void             SThrow  (const PSQL::ErrorResponseDescRS&   aMsgDescOut,
                                     const SourceLocationT&             aSourceLocation = SourceLocationT::current());
};

}// namespace GPlatform::PSQL

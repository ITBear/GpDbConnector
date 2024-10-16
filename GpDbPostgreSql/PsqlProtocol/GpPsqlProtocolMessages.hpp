#pragma once

#include <GpCore2/GpUtils/Types/Numerics/GpNumericTypes.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpBytesArray.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>

#include <variant>

namespace GPlatform::PSQL {

// https://www.postgresql.org/docs/current/protocol-overview.html
// https://www.postgresql.org/docs/current/protocol-message-formats.html

enum class TypeOID: u_int_32
{
    INT2            = 21,
    INT2_ARRAY      = 22,
    INT4            = 23,
    INT4_ARRAY      = 1007,
    INT8            = 20,
    INT8_ARRAY      = 1016,
    FLOAT4          = 700,
    FLOAT4_ARRAY    = 1021,
    FLOAT8          = 701,
    FLOAT8_ARRAY    = 1022,
    TEXT            = 25,
    TEXT_ARRAY      = 1009,
    UUID            = 2950,
    UUID_ARRAY      = 2951,
    BYTEA           = 17,
    BYTEA_ARRAY     = 1001,
    BOOL            = 16,
    BOOL_ARRAY      = 1000
};

enum class MessageRqDescId: u_int_8
{
    NO_MESSAGE              = 0,
    STARTUP_MESSAGE         = 255,
    PASSWORD_MESSAGE        = 'p',
    QUERY                   = 'Q', // The client sends this message with the SQL statement as a string. The server processes the query and sends back the result or an error.
    TERMINATE               = 'X',
    PARSE                   = 'P', // The client sends this message to parse an SQL statement and create a prepared statement on the server. You can include parameter placeholders ($1, $2, etc.) in the SQL statement.
    BIND                    = 'B', // Binds the parameters to the prepared statement.
    EXECUTE                 = 'E', // Executes the prepared statement with the bound parameters.
    CLOSE                   = 'C',
    FLUSH                   = 'H',
    SYNC                    = 'S', // Indicates that the client is finished sending commands for now.
                                   // The client sends this message to synchronize with the server and receive any pending responses. It usually follows a QUERY or EXECUTE.
    FUNCTION_CALL           = 'F',
    COPY_DATA               = 'd',
    COPY_DONE               = 'c',
    COPY_FAIL               = 'f'
};

enum class MessageRsDescId
{
    AUTHENTICATION_REQUEST  = 'R', // The client receives an AUTHENTICATION_REQUEST message during the initial connection phase, immediately after the client sends the startup message to the server. This message is part of the server's response to the client's attempt to authenticate.
    BACKEND_KEY_DATA        = 'K', // The client receives a BACKEND_KEY_DATA message during the connection startup process, immediately after the AuthenticationOk message is sent by the server.
    BIND_COMPLETE           = '2',
    CLOSE_COMPLETE          = '3',
    COMMAND_COMPLETE        = 'C',
    COPY_IN_RESPONSE        = 'G',
    COPY_OUT_RESPONSE       = 'H',
    DATA_ROW                = 'D',
    ERROR_RESPONSE          = 'E', // The client receives an ERROR_RESPONSE message whenever an error occurs during the communication between the client and the server. This message is sent by the server to notify the client that a problem has occurred that prevents the server from successfully processing a request.
    FUNCTION_CALL_RESPONSE  = 'V',
    EMPTY_DATA              = 'n', // NO_DATA
    NOTICE_RESPONSE         = 'N',
    NOTIFICATION_RESPONSE   = 'A',
    PARAMETER_DESCRIPTION   = 't',
    PARAMETER_STATUS        = 'S', // The client receives a PARAMETER_STATUS message during the startup phase of a connection and whenever a run-time parameter changes during the session.
    PARSE_COMPLETE          = '1',
    PORTAL_SUSPENDED        = 's',
    READY_FOR_QUERY         = 'Z', // The client receives a READY_FOR_QUERY message from the server after the server has completed processing the previous command and is ready to accept a new command. This message is an indication that the server is idle and waiting for the next request from the client.
    ROW_DESCRIPTION         = 'T'
};

enum class AuthenticationMethod
{
    AUTH_OK             = 0,
    KERBEROS_V5         = 2,
    CLEAR_TEXT_PASSWORD = 3,
    MD5_PASSWORD        = 5,
    GSS                 = 7,
    SSPI                = 9,
    SASL                = 10,
    SASL_CONTINUE       = 11,
    SASL_FINAL          = 12
};

enum class ErrorCodeId
{
    SEVERITY_1              = 'S',  // 'Severity': The field contents are ERROR, FATAL, or PANIC (in an error message), or WARNING, NOTICE, DEBUG, INFO, or LOG (in a notice message),
                                    // or a localized translation of one of these. Always present.
    SEVERITY_2              = 'V',  // 'Severity': The field contents are ERROR, FATAL, or PANIC (in an error message), or WARNING, NOTICE, DEBUG, INFO, or LOG (in a notice message). T
                                    // his is identical to the S field except that the contents are never localized. This is present only in messages generated by
                                    // PostgreSQL versions 9.6 and later.
    SQLSTATE_CODE           = 'C',  // 'Code': The SQLSTATE code for the error (see Appendix A). Not localizable. Always present.
    MESSAGE                 = 'M',  // 'Message': The primary human-readable error message. This should be accurate but terse (typically one line). Always present.
    DETAIL                  = 'D',  // 'Detail': An optional secondary error message carrying more detail about the problem. Might run to multiple lines.
    HINT                    = 'H',  // 'Hint': An optional suggestion what to do about the problem. This is intended to differ from Detail in that it offers advice (potentially inappropriate) rather than hard facts. Might run to multiple lines.
    POSITION                = 'P',  // 'Position': The field value is a decimal ASCII integer, indicating an error cursor position as an index into the original query string.
                                    // The first character has index 1, and positions are measured in characters not bytes.
    INTERNAL_POSITION       = 'p',  // 'Internal position': This is defined the same as the P field, but it is used when the cursor position refers to an internally generated command rather than the one submitted by the client.
                                    // The q field will always appear when this field appears.
    INTERNAL_QUERY          = 'q',  // 'Internal query': The text of a failed internally-generated command. This could be, for example, an SQL query issued by a PL/pgSQL function.
    WHERE                   = 'W',  // 'Where': An indication of the context in which the error occurred. Presently this includes a call stack traceback of active
                                    // procedural language functions and internally-generated queries. The trace is one entry per line, most recent first.
    SCHEMA_NAME             = 's',  // 'Schema name': if the error was associated with a specific database object, the name of the schema containing that object, if any.
    TABLE_NAME              = 't',  // 'Table name': if the error was associated with a specific table, the name of the table. (Refer to the schema name field for the name of the table's schema.)
    COLUMN_NAME             = 'c',  // 'Column name': if the error was associated with a specific table column, the name of the column. (Refer to the schema and table name fields to identify the table.)
    DATA_TYPE_NAME          = 'd',  // 'Data type name': Data type name: if the error was associated with a specific data type, the name of the data type.
                                    // (Refer to the schema name field for the name of the data type's schema.)
    CONSTRAINT_NAME         = 'n',  // 'Constraint name': if the error was associated with a specific constraint, the name of the constraint. Refer to fields listed above for the associated table or domain.
                                    // (For this purpose, indexes are treated as constraints, even if they weren't created with constraint syntax.)
    FILE                    = 'F',  // 'File': the file name of the source-code location where the error was reported.
    LINE                    = 'L',  // 'Line': the line number of the source-code location where the error was reported.
    ROUTINE                 = 'R'   // 'Routine': the name of the source-code routine reporting the error.
};

enum class TransactionStatus
{
    IDLE,
    TRANSACTION_BLOCK,
    FAILED_TRANSACTION_BLOCK
};

// -------------------------------- RQ messages --------------------------------

struct StartupMessageDescRQ
{
    using AttributesMapT = boost::container::small_flat_map<std::string, std::string, 8>;

    // For historical reasons, the very first message sent by the client (the startup message) has no initial message-type byte.
    MessageRqDescId message_id  = PSQL::MessageRqDescId::STARTUP_MESSAGE;
    //u_int_32      length;              // Total length of the message (no need here, caclulated in ProtocolSerializer)
    u_int_32        protocol_version;    // Protocol version number
    AttributesMapT  attributes;          // Series of null-terminated key-value pairs
};

struct QueryDescRQ
{
    MessageRqDescId message_id  = PSQL::MessageRqDescId::QUERY;
    std::string     query;                  // The query string
};

struct SASLInitialResponseDescRQ
{
    MessageRqDescId message_id  = MessageRqDescId::PASSWORD_MESSAGE;
    //u_int_32      length;                 // Total length of the message (no need here, caclulated in ProtocolSerializer)
    std::string     name;                   // Name of the SASL authentication mechanism that the client selected.
    std::string     client_first_message;   // SASL mechanism specific "Initial Response". (u_int_32 length + payload)
};

struct SASLResponseDescRQ
{
    MessageRqDescId message_id  = MessageRqDescId::PASSWORD_MESSAGE;
    //u_int_32      length;                 // Total length of the message (no need here, caclulated in ProtocolSerializer)
    std::string     client_final_message;   // SASL mechanism specific (only payload bytes without size)
};

// -------------------------------- RS messages --------------------------------
// The server then immediately closes the connection.
struct ErrorResponseDescRS
{
    using MessagesT = std::vector<std::tuple<ErrorCodeId, std::string_view>>;

    u_int_32        length = 0;

    // The message body consists of one or more identified fields, followed by a zero byte as a terminator. Fields can appear in any order.
    // For each field there is the following:
    // 1. Byte1: A code identifying the field type; if zero, this is the message terminator and no string follows.
    //           The presently defined field types are listed in Section 55.8. Since more field types might be added in future,
    //           frontends should silently ignore fields of unrecognized type.
    // 2. String: The field value.
    MessagesT       messages;
};

//
struct AuthenticationDescRS
{
    struct AuthenticationOk
    {
        // Empty
    };

    struct AuthenticationSASL
    {
        std::vector<std::string> names;     // Name of a SASL authentication mechanism. (null terminated strings)
    };

    struct AuthenticationSASLContinue
    {
        std::string server_first_message;   // SASL mechanism specific (only payload bytes without size)
    };

    struct AuthenticationSASLFinal
    {
        std::string server_final_message;   // SASL mechanism specific (only payload bytes without size)
    };

    using PayloadT = std::variant
    <
        AuthenticationOk,
        AuthenticationSASL,
        AuthenticationSASLContinue,
        AuthenticationSASLFinal
    >;

    u_int_32                length = 0;     // Total length of the message
    AuthenticationMethod    auth_method;    // Type of authentication required
    PayloadT                payload;
};

// run-time parameter status report
struct ParameterStatusDescRS
{
    u_int_32        length = 0;     // Total length of the message
    std::string     name;           // The name of the run-time parameter being reported
    std::string     value;          // The current value of the parameter
};

// Cancellation key data. The frontend must save these values if it wishes to be able to issue CancelRequest messages later.
struct BackendKeyDataDescRS
{
    u_int_32        length  = 0;    // Total length of the message
    u_int_32        pid     = 0;    // The process ID of this backend
    u_int_32        secret  = 0;    // The secret key of this backend
};

//
struct ReadyForQueryDescRS
{
    u_int_32            length              = 0;                        // Total length of the message
    TransactionStatus   transaction_status  = TransactionStatus::IDLE;  // Current backend transaction status indicator. Possible values are 'I' if idle (not in a transaction block);
                                                                        // 'T' if in a transaction block; or 'E' if in a failed transaction block (queries will be rejected until block is ended).
};

// RowDescription
struct RowDescriptionDescRS
{
    struct ColumnDesc
    {
        using Vec = boost::container::small_vector<ColumnDesc, 16>;

        std::string name;                       // Field Name: (null-terminated string)
        u_int_32    table_oid           = 0;    // The object ID of the table from which this field was retrieved. If this field is an expression rather than a direct
                                                // reference to a table column, the value is 0.
        u_int_16    attribute_number    = 0;    // The attribute number of the column. If the field is an expression or does not directly correspond to a table column,
                                                // the value is 0.
        u_int_32    type_oid            = 0;    // The OID of the data type of the column.
        u_int_16    type_size           = 0;    // The size of the data type. This is the size in bytes, or -1 for variable-length types.
        u_int_32    type_modifier       = 0;    // The type modifier is typically used to specify precision or scale for numeric types.
        u_int_16    format_code         = 0;    // Specifies the format of the field: 0 indicates the field is in text format, 1 indicates the field is in binary format
    };

    void Clear (void)
    {
        columns.clear();
    }

    u_int_32        length  = 0;    // Total length of the message
    ColumnDesc::Vec columns;        // Columns
};

// DataRow
struct DataRowDescRS
{
    using ColumnDescVec = boost::container::small_vector<GpSpanByteR, 16>;

    u_int_32        length  = 0;    // Total length of the message
    ColumnDescVec   columns;
};

// CommandComplete
struct CommandCompleteDescRS
{
    u_int_32            length  = 0;    // Total length of the message
    std::string_view    command;        // The command tag. This is usually a single word that identifies which SQL command was completed.
};

}// namespace GPlatform::PSQL

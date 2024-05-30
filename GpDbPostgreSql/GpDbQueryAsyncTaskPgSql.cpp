#include <GpDbConnector/GpDbPostgreSql/GpDbQueryAsyncTaskPgSql.hpp>
#include <GpDbConnector/GpDbClient/GpDbException.hpp>
#include <GpDbConnector/GpDbPostgreSql/GpDbQueryResPgSql.hpp>
#include <GpDbConnector/GpDbPostgreSql/GpDbQueryPreparedPgSql.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>

namespace GPlatform {

GpDbQueryAsyncTaskPgSql::GpDbQueryAsyncTaskPgSql
(
    GpDbConnectionPgSql&        aDbConn,
    const GpDbQuery&            aQuery,
    const GpDbQueryPrepared&    aQueryPrepared,
    const GpIOEventPollerIdx    aIOEventPollerIdx
):
GpSingleSocketTask
{
    GpSocketTCP::SFromID
    (
        aDbConn.SocketId(),
        GpSocket::CloseModeT::KEEP_ON_DESTRUCT,
        GpSocketTCP::StateT::INCOMING
    ),
    aIOEventPollerIdx
},
iDbConn       {aDbConn},
iQuery        {aQuery},
iQueryPrepared{aQueryPrepared}
{
}

GpDbQueryAsyncTaskPgSql::~GpDbQueryAsyncTaskPgSql (void) noexcept
{
}

void    GpDbQueryAsyncTaskPgSql::OnReadyToRead (GpSocket& /*aSocket*/)
{
    // Get connection
    PGconn* pgConn = iDbConn.PgConn();

    // Consume input
    const int rc = PQconsumeInput(pgConn);

    THROW_COND_DB
    (
        rc != 0,
        GpDbExceptionCode::CONNECTION_ERROR,
        "PQconsumeInput return error: "_sv + PQerrorMessage(pgConn)
    );

    // Consume and ignore PGnotify
    {
        PGnotify* pgNotify = nullptr;
        while ((pgNotify = PQnotifies(pgConn)) != nullptr)
        {
            PQfreemem(pgNotify);
        }
    }

    // Check if connection is busy (IO operations)
    if (PQisBusy(pgConn))
    {
        return;
        //return GpTaskRunRes::WAIT;
    }

    // Get result
    PGresult* pgResult = PQgetResult(pgConn);
    DonePromise().Fulfill(MakeSP<GpDbQueryResPgSql>(pgResult));

    // TODO: manualy call task stop request;
    THROW_GP_NOT_IMPLEMENTED();
    //return GpTaskRunRes::DONE;
}

void    GpDbQueryAsyncTaskPgSql::OnReadyToWrite (GpSocket& /*aSocket*/)
{
    if (iIsSend)
    {
        return;// GpTaskRunRes::WAIT;
    }

    const GpDbQueryPreparedPgSql&   queryPrepared   = static_cast<const GpDbQueryPreparedPgSql&>(iQueryPrepared);
    std::string_view                queryStr        = iQuery.QueryStr();
    std::string                     queryZT;

    queryZT.reserve(NumOps::SAdd<size_t>(queryStr.length(), 1));
    queryZT.append(queryStr).append("\0"_sv);

    // NOTE: Can be blocked until all data is written to the socket
    const int sendRes = PQsendQueryParams
    (
        iDbConn.PgConn(),
        std::data(queryZT),
        int(std::size(iQuery.Values())),
        std::data(queryPrepared.OIDs()),
        std::data(queryPrepared.ValuesPtr()),
        std::data(queryPrepared.ValuesSize()),
        std::data(queryPrepared.ValuesIsBinary()),
        int(GpPosrgresQueryResultType::BINARY)
    );

    THROW_COND_DB
    (
        sendRes == 1,
        GpDbExceptionCode::QUERY_ERROR,
        [&](){return "Failed to do SQL query: "_sv + PQerrorMessage(iDbConn.PgConn());}
    );

    iIsSend = true;
}

void    GpDbQueryAsyncTaskPgSql::OnClosed (GpSocket& /*aSocket*/)
{
    THROW_DB
    (
        GpDbExceptionCode::QUERY_ERROR,
        "Failed to do SQL query: socket closed"_sv
    );
}

void    GpDbQueryAsyncTaskPgSql::OnError (GpSocket& /*aSocket*/)
{
    THROW_DB
    (
        GpDbExceptionCode::QUERY_ERROR,
        "Failed to do SQL query: socket error"_sv
    );
}

void    GpDbQueryAsyncTaskPgSql::ProcessOtherMessages (GpAny& aMessage)
{
    THROW_GP
    (
        fmt::format
        (
            "Get not socket message {}",
            aMessage.TypeInfo().name()
        )
    );
}

}// namespace GPlatform

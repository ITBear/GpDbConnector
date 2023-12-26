#include "GpDbQueryAsyncTaskPgSql.hpp"
#include "../../GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp"
#include "../GpDbClient/GpDbException.hpp"
#include "GpDbQueryResPgSql.cpp"
#include "GpDbQueryPreparedPgSql.hpp"

namespace GPlatform {

GpDbQueryAsyncTaskPgSql::GpDbQueryAsyncTaskPgSql
(
    GpDbConnectionPgSql&        aDbConn,
    const GpDbQuery&            aQuery,
    const GpDbQueryPrepared&    aQueryPrepared
):
GpSingleSocketTask
(
    GpSocketTCP::SFromID
    (
        aDbConn.SocketId(),
        GpSocket::CloseModeT::KEEP_ON_DESTRUCT,
        GpSocketTCP::StateT::INCOMING
    )
),
iDbConn       (aDbConn),
iQuery        (aQuery),
iQueryPrepared(aQueryPrepared)
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
        u8"PQconsumeInput return error: "_sv + GpUTF::S_As_UTF8(PQerrorMessage(pgConn))
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
    std::u8string_view              queryStr        = iQuery.QueryStr();
    std::u8string                   queryZT;

    queryZT.reserve(NumOps::SAdd<size_t>(queryStr.length(), 1));
    queryZT.append(queryStr).append(u8"\0"_sv);

    // NOTE: Can be blocked until all data is written to the socket
    const int sendRes = PQsendQueryParams
    (
        iDbConn.PgConn(),
        GpUTF::S_As_STR(queryZT).data(),
        int(iQuery.Values().size()),
        queryPrepared.OIDs().data(),
        queryPrepared.ValuesPtr().data(),
        queryPrepared.ValuesSize().data(),
        queryPrepared.ValuesIsBinary().data(),
        int(GpPosrgresQueryResultType::BINARY)
    );

    THROW_COND_DB
    (
        sendRes == 1,
        GpDbExceptionCode::QUERY_ERROR,
        [&](){return u8"Failed to do SQL query: "_sv + GpUTF::S_As_UTF8(PQerrorMessage(iDbConn.PgConn()));}
    );

    iIsSend = true;

    return;// GpTaskRunRes::WAIT;
}

void    GpDbQueryAsyncTaskPgSql::OnClosed (GpSocket& /*aSocket*/)
{
    THROW_DB
    (
        GpDbExceptionCode::QUERY_ERROR,
        u8"Failed to do SQL query: socket closed"_sv
    );
}

void    GpDbQueryAsyncTaskPgSql::OnError (GpSocket& /*aSocket*/)
{
    THROW_DB
    (
        GpDbExceptionCode::QUERY_ERROR,
        u8"Failed to do SQL query: socket error"_sv
    );
}

}// namespace GPlatform

#include "GpDbQueryAsyncTaskPgSql.hpp"
#include "GpDbQueryResPgSql.hpp"
#include "../GpDbClient/GpDbException.hpp"
#include "../../GpNetwork/GpNetworkCore/IO/Sockets/GpSocketTCP.hpp"

namespace GPlatform {

GpDbQueryAsyncTaskPgSql::GpDbQueryAsyncTaskPgSql
(
    std::u8string               aName,
    GpDbConnectionPgSql&        aDbConn,
    const GpDbQuery&            aQuery,
    const GpDbQueryPrepared&    aQueryPrepared
    //const size_t              aMinResultRowsCount
):
GpSocketTask
(
    std::move(aName),
    aDbConn.EventPoller(),
    GpSocketTCP::SFromID
    (
        aDbConn.SocketId(),
        GpSocket::CloseModeT::NO_CLOSE_ON_DESTRUCT,
        GpSocketTCP::StateT::INCOMING
    )
),
iDbConn(aDbConn),
iQuery(aQuery),
iQueryPrepared(aQueryPrepared)
//iMinResultRowsCount(aMinResultRowsCount)
{
}

GpDbQueryAsyncTaskPgSql::~GpDbQueryAsyncTaskPgSql (void) noexcept
{
}

GpTaskDoRes GpDbQueryAsyncTaskPgSql::OnSockReadyToRead (GpSocket& /*aSocket*/)
{
    try
    {
        return ProcessR();
    } catch (const GpException& e)
    {
        CompletePromise(MakeSP<CompleteItcResultT>(e));
        throw;
    } catch (const std::exception& e)
    {
        CompletePromise(MakeSP<CompleteItcResultT>(e));
        throw;
    } catch (...)
    {
        //CompletePromise(MakeSP<CompleteItcResultT>(std::runtime_error("Unknown exception")));
        CompletePromise(MakeSP<CompleteItcResultT>(GpException(u8"Unknown exception")));
        throw;
    }
}

GpTaskDoRes GpDbQueryAsyncTaskPgSql::OnSockReadyToWrite (GpSocket& /*aSocket*/)
{
    try
    {
        return ProcessW();
    } catch (const GpException& e)
    {
        CompletePromise(MakeSP<CompleteItcResultT>(e));
        throw;
    } catch (const std::exception& e)
    {
        CompletePromise(MakeSP<CompleteItcResultT>(e));
        throw;
    } catch (...)
    {
        //CompletePromise(MakeSP<CompleteItcResultT>(std::runtime_error("Unknown exception")));
        CompletePromise(MakeSP<CompleteItcResultT>(GpException(u8"Unknown exception")));
        throw;
    }
}

void    GpDbQueryAsyncTaskPgSql::OnSockClosed (GpSocket& /*aSocket*/)
{
    //CompletePromise(MakeSP<CompleteItcResultT>(std::runtime_error("Socket closed")));
    CompletePromise(MakeSP<CompleteItcResultT>(GpException(u8"Socket closed")));
}

void    GpDbQueryAsyncTaskPgSql::OnSockError (GpSocket& /*aSocket*/)
{
    //CompletePromise(MakeSP<CompleteItcResultT>(std::runtime_error("Socket error")));
    CompletePromise(MakeSP<CompleteItcResultT>(GpException(u8"Socket error")));
}

GpTaskDoRes GpDbQueryAsyncTaskPgSql::ProcessR (void)
{
    PGconn* pgConn = iDbConn.PgConn();

    const int rc = PQconsumeInput(pgConn);

    if (rc == 0)
    {
        CompletePromise
        (
            MakeSP<CompleteItcResultT>
            (
                GpException(u8"PQconsumeInput return error: "_sv + GpUTF::S_STR_To_UTF8(PQerrorMessage(pgConn)))
            )
        );

        return GpTaskDoRes::DONE;
    }

    PGnotify* pgNotify = nullptr;

    while ((pgNotify = PQnotifies(pgConn)) != nullptr)
    {
        PQfreemem(pgNotify);
    }

    if (PQisBusy(pgConn) == 0)
    {
        PGresult* pgResult = nullptr;

        while ((pgResult = PQgetResult(pgConn)) != nullptr)
        {
            CompletePromise(MakeSP<CompleteItcResultT>(size_t(0)));
            //TODO: reimplement
            //CompletePromise(MakeSP<CompleteItcResultT>(MakeSP<GpDbQueryResPgSql>(pgResult)));
        }

        return GpTaskDoRes::DONE;
    }

    return GpTaskDoRes::WAITING;
}

GpTaskDoRes GpDbQueryAsyncTaskPgSql::ProcessW (void)
{
    if (iIsSend == false)
    {
        Send();
        iIsSend = true;
    }

    return GpTaskDoRes::WAITING;
}

void    GpDbQueryAsyncTaskPgSql::Send (void)
{
    const GpDbQueryPreparedPgSql&   queryPrepared   = static_cast<const GpDbQueryPreparedPgSql&>(iQueryPrepared);
    std::u8string_view              queryStr        = iQuery.QueryStr();
    std::u8string                   queryZT;

    queryZT.reserve(NumOps::SAdd<size_t>(queryStr.length(), 1));
    queryZT.append(queryStr).append(u8"\0"_sv);

    const int sendRes = PQsendQueryParams
    (
        iDbConn.PgConn(),
        GpUTF::S_UTF8_To_STR(queryZT).data(),
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
        [&](){return u8"Failed to do SQL query: "_sv + GpUTF::S_STR_To_UTF8(PQerrorMessage(iDbConn.PgConn()));}
    );
}

}//namespace GPlatform

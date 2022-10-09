#include "GpDbQueryAsyncTaskPgSql.hpp"
#include "GpDbQueryResPgSql.hpp"

namespace GPlatform {

GpDbQueryAsyncTaskPgSql::GpDbQueryAsyncTaskPgSql
(
    std::string                 aName,
    GpDbConnectionPgSql&        aDbConn,
    const GpDbQuery&            aQuery,
    const GpDbQueryPrepared&    aQueryPrepared,
    const size_t                aMinResultRowsCount
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
iQueryPrepared(aQueryPrepared),
iMinResultRowsCount(aMinResultRowsCount)
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
        CompletePromise(MakeSP<GpItcResult>(e));
        throw;
    } catch (const std::exception& e)
    {
        CompletePromise(MakeSP<GpItcResult>(e));
        throw;
    } catch (...)
    {
        CompletePromise(MakeSP<GpItcResult>(std::runtime_error("Unknown exception")));
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
        CompletePromise(MakeSP<GpItcResult>(e));
        throw;
    } catch (const std::exception& e)
    {
        CompletePromise(MakeSP<GpItcResult>(e));
        throw;
    } catch (...)
    {
        CompletePromise(MakeSP<GpItcResult>(std::runtime_error("Unknown exception")));
        throw;
    }
}

void    GpDbQueryAsyncTaskPgSql::OnSockClosed (GpSocket& /*aSocket*/)
{
    CompletePromise(MakeSP<GpItcResult>(std::runtime_error("Socket closed")));
}

void    GpDbQueryAsyncTaskPgSql::OnSockError (GpSocket& /*aSocket*/)
{
    CompletePromise(MakeSP<GpItcResult>(std::runtime_error("Socket error")));
}

GpTaskDoRes GpDbQueryAsyncTaskPgSql::ProcessR (void)
{
    PGconn* pgConn = iDbConn.PgConn();

    const int rc = PQconsumeInput(pgConn);

    if (rc == 0)
    {
        CompletePromise(MakeSP<GpItcResult>(GpException("PQconsumeInput return error: "_sv + PQerrorMessage(pgConn))));
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
            CompletePromise(MakeSP<GpItcResult>(MakeSP<GpDbQueryResPgSql>(pgResult)));
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
    std::string_view                queryStr        = iQuery.QueryStr();
    std::string                     queryZT;

    queryZT.reserve(NumOps::SAdd<size_t>(queryStr.length(), 1));
    queryZT.append(queryStr).append("\0"_sv);

    const int sendRes = PQsendQueryParams
    (
        iDbConn.PgConn(),
        queryZT.data(),
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
        [&](){return "Failed to do SQL query: "_sv + std::string_view(PQerrorMessage(iDbConn.PgConn()));}
    );
}

}//namespace GPlatform

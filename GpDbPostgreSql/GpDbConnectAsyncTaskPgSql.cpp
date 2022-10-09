#include "GpDbConnectAsyncTaskPgSql.hpp"

namespace GPlatform {

GpDbConnectAsyncTaskPgSql::GpDbConnectAsyncTaskPgSql
(
    std::string         aName,
    GpIOEventPoller::SP aIOPoller,
    PGconn*             aPGconn
):
GpSocketTask
(
    std::move(aName),
    std::move(aIOPoller),
    GpSocketTCP::SFromID
    (
        PQsocket(aPGconn),
        GpSocket::CloseModeT::NO_CLOSE_ON_DESTRUCT,
        GpSocketTCP::StateT::INCOMING
    )
),
iPGconn(aPGconn)
{
}

GpDbConnectAsyncTaskPgSql::~GpDbConnectAsyncTaskPgSql (void) noexcept
{
}

GpTaskDoRes GpDbConnectAsyncTaskPgSql::OnSockReadyToRead (GpSocket& /*aSocket*/)
{
    return ProcessRW();
}

GpTaskDoRes GpDbConnectAsyncTaskPgSql::OnSockReadyToWrite (GpSocket& /*aSocket*/)
{
    return ProcessRW();
}

void    GpDbConnectAsyncTaskPgSql::OnSockClosed (GpSocket& /*aSocket*/)
{
    CompletePromise(MakeSP<GpItcResult>(GpDbConnectionStatus(GpDbConnectionStatus::CLOSED)));
}

void    GpDbConnectAsyncTaskPgSql::OnSockError (GpSocket& /*aSocket*/)
{
    CompletePromise(MakeSP<GpItcResult>(GpDbConnectionStatus(GpDbConnectionStatus::CLOSED)));
}

GpTaskDoRes GpDbConnectAsyncTaskPgSql::ProcessRW (void)
{
    PostgresPollingStatusType pgPoolRes = PQconnectPoll(iPGconn);

    if (pgPoolRes == PGRES_POLLING_FAILED)
    {
        CompletePromise(MakeSP<GpItcResult>(GpDbConnectionStatus(GpDbConnectionStatus::CLOSED)));
        return GpTaskDoRes::DONE;
    } else if (pgPoolRes == PGRES_POLLING_OK)
    {
        CompletePromise(MakeSP<GpItcResult>(GpDbConnectionStatus(GpDbConnectionStatus::CONNECTED)));
        return GpTaskDoRes::DONE;
    }

    //PQconnectPoll(iPGconn);

    //Return to waiting
    return GpTaskDoRes::WAITING;
}

}//namespace GPlatform

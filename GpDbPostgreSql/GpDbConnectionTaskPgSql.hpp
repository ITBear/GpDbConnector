#pragma once

#include <GpNetwork/GpNetworkCore/Tasks/GpTcpClientTask.hpp>
#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlProtocolMessages.hpp>
#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlScramSha256.hpp>
#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlMessageProcessor.hpp>
#include <GpDbConnector/GpDbClient/Query/GpDbQueryRes.hpp>
#include <GpDbConnector/GpDbClient/Query/GpDbQuery.hpp>

namespace GPlatform {

class GpDbConnectionTaskPgSql final: public GpTcpClientTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbConnectionTaskPgSql)
    CLASS_DD(GpDbConnectionTaskPgSql)

    enum class ProcessStateT
    {
        CHECK_CONNECTION,
        IDLE,
        SEND_MESSAGE
    };

    class ConnectedToDbMsg {};
    using ConnectedToDbPromiseT = GpItcSharedPromise<ConnectedToDbMsg>;
    using ConnectedToDbFutureT  = typename ConnectedToDbPromiseT::FutureT;

    using ExecutePromiseT       = GpItcSharedPromise<GpDbQueryRes::SP>;
    using ExecuteFutureT        = typename ExecutePromiseT::FutureT;
    using ExecuteMsgT           = std::tuple<const GpDbQuery&, size_t/*aMinResultRowsCount*/, ExecutePromiseT>;

public:
                                    GpDbConnectionTaskPgSql     (GpIOEventPollerIdx aIOEventPollerIdx,
                                                                 milliseconds_t     aConnectTimeout,
                                                                 std::string        aServerHost,
                                                                 u_int_16           aServerPort,
                                                                 std::string        aUserName,
                                                                 std::string        aPassword,
                                                                 std::string        aDatabase);
    virtual                         ~GpDbConnectionTaskPgSql    (void) noexcept override final;

    bool                            IsConnected                 (void) const noexcept {return iIsConnectd.load(std::memory_order_relaxed);}
    ConnectedToDbFutureT::SP        GetConnectedToDbFuture      (void);

    GpDbQueryRes::SP                Execute                     (const GpDbQuery&   aQuery,
                                                                 size_t             aMinResultRowsCount);

protected:
    virtual void                    OnStart                     (void) override final;
    virtual void                    OnStop                      (StopExceptionsT& aStopExceptionsOut) noexcept override final;
    virtual void                    OnStopException             (const GpException& aException) noexcept override final;

    virtual void                    OnReadyToRead               (GpSocket& aSocket) override final;
    virtual void                    OnReadyToWrite              (GpSocket& aSocket) override final;
    virtual void                    OnClosed                    (GpSocket& aSocket) override final;
    virtual void                    OnError                     (GpSocket& aSocket) override final;
    virtual void                    OnConnected                 (GpSocketTCP& aSocket) override final;
    virtual void                    ProcessOtherMessages        (GpAny& aMessage) override;

private:
    void                            ReadFromSocket              (GpSocketTCP& aSocket);
    void                            WriteToSocket               (GpSocketTCP& aSocket);
    void                            PrepareAndSendMessage       (size_t aMessageSize);
    void                            ProcessRsMessage            (GpSpanByteR aMessageData);

    void                            OnAuthOk                    (void);

private:
    const milliseconds_t            iConnectTimeout;
    const std::string               iServerHost;
    const u_int_16                  iServerPort;

    std::atomic_bool                iIsConnectd             = false;
    ConnectedToDbPromiseT           iConnectedToDbPromise;

    ProcessStateT                   iProcessState           = ProcessStateT::CHECK_CONNECTION;

    GpBytesArray                    iSocketTmpBufferRead;
    GpBytesArray                    iSocketTmpBufferWrite;
    size_t                          iRqBytesToWriteTotal    = 0;
    size_t                          iRqBytesWrited          = 0;
    size_t                          iRsSizeOfCurrentMessage = 0;

    PSQL::GpPsqlMessageProcessor    iMessageProcessor;
};

}// namespace GPlatform


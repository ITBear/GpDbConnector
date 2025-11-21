#pragma once

#include <GpNetwork/GpNetworkCore/Tasks/GpTcpClientTask.hpp>
#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlProtocolMessages.hpp>
#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlScramSha256.hpp>
#include <GpDbConnector/GpDbPostgreSql/PsqlProtocol/GpPsqlMessageProcessor.hpp>
#include <GpDbConnector/GpDbClient/Query/GpDbQueryRes.hpp>
#include <GpDbConnector/GpDbClient/Query/GpDbQuery.hpp>
#include <GpDbConnector/GpDbPostgreSql/GpDbQueryResPgSql.hpp>

namespace GPlatform {

class GpDbConnectionTaskPgSql final: public GpTcpClientTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbConnectionTaskPgSql)
    CLASS_DD(GpDbConnectionTaskPgSql)

    enum class SocketStateT
    {
        CONNECTING,
        IDLE,
        SENDING_MESSAGE
    };

    class ConnectedToDbMsg {};
    using ConnectedToDbPromiseT = GpItcPromise<ConnectedToDbMsg>;
    using ConnectedToDbFutureT  = typename ConnectedToDbPromiseT::FutureT;

    using ExecutePromiseT       = GpItcPromise<GpDbQueryRes::SP>;
    using ExecuteFutureT        = typename ExecutePromiseT::FutureT;
    using ExecuteMsgT           = std::tuple<const GpDbQuery&, ExecutePromiseT>;

public:
                                GpDbConnectionTaskPgSql     (GpSocketTCP::UP    aSocketTcpUP,
                                                             milliseconds_t     aConnectTimeout,
                                                             std::string        aServerHost,
                                                             u_int_16           aServerPort,
                                                             std::string        aUserName,
                                                             std::string        aPassword,
                                                             std::string        aDatabase);
    virtual                     ~GpDbConnectionTaskPgSql    (void) noexcept override final;

    bool                        IsConnected                 (void) const noexcept {return iIsConnectd.load(std::memory_order_relaxed);}
    ConnectedToDbFutureT::SP    GetConnectedToDbFuture      (void);

    GpDbQueryRes::SP            Execute                     (const GpDbQuery& aQuery);

protected:
    virtual void                OnStart                     (void) override final;
    virtual void                OnStop                      (ExceptionsT& aStopExceptionsOut) noexcept override final;
    virtual void                OnStopException             (const GpException& aException) noexcept override final;

    virtual void                OnReadyToRead               (GpSocket& aSocket) REQUIRES(SpinLock()) override final;
    virtual void                OnReadyToWrite              (GpSocket& aSocket) REQUIRES(SpinLock()) override final;
    virtual void                OnClosed                    (GpSocket& aSocket) REQUIRES(SpinLock()) override final;
    virtual void                OnError                     (GpSocket& aSocket) REQUIRES(SpinLock()) override final;
    virtual void                OnConnected                 (GpSocketTCP& aSocket) REQUIRES(SpinLock()) override final;
    virtual void                OnConnectionTimeout         (GpSocketTCP& aSocket) REQUIRES(SpinLock()) override final;

    void                        ProcessExecuteMsg           (ExecuteMsgT& aMessage);

private:
    void                        ReadFromSocket              (GpSocketTCP& aSocket);
    void                        WriteToSocket               (GpSocketTCP& aSocket);
    void                        PrepareAndSendMessage       (size_t aMessageSize);
    void                        ProcessRsMessage            (GpSpanByteR aMessageData);

    void                        OnReadyForRequest           (void);
    void                        OnDataRow                   (const PSQL::RowDescriptionDescRS&  aRowDesc,
                                                             const PSQL::DataRowDescRS&         aRowData);
    void                        OnCommandComplete           (const PSQL::CommandCompleteDescRS& aCommandCompleteDesc);

    std::vector<PSQL::TypeOID>  QueryToOIDs                 (const GpDbQuery::ValueVecT& aValues) const;

private:
    const milliseconds_t            iConnectTimeout;
    const std::string               iServerHost;
    [[maybe_unused]] const u_int_16 iServerPort;

    std::atomic_bool                iIsConnectd                 = false;
    ConnectedToDbPromiseT           iConnectedToDbPromise;

    SocketStateT                    iSocketState                = SocketStateT::IDLE;
    ExecutePromiseT                 iExecutePromise;
    GpDbQueryResPgSql::SP           iDbQueryResSP;

    // Read buffers
    GpByteArray                     iSocketTmpBufferRead;
    size_t                          iSocketTmpBufferReadSize    = 0;

    // Write buffers
    GpByteArray                     iSocketTmpBufferWrite;
    size_t                          iRqBytesToWriteTotal        = 0;
    size_t                          iRqBytesWrited              = 0;

    // Message processor
    PSQL::GpPsqlMessageProcessor    iMessageProcessor;
};

}// namespace GPlatform


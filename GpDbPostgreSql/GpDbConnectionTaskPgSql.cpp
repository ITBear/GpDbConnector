#include <GpDbConnector/GpDbPostgreSql/GpDbConnectionTaskPgSql.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>
#include <GpCore2/GpUtils/Debugging/GpDebugging.hpp>
#include <GpDbConnector/GpDbClient/GpDbException.hpp>
#include <GpDbConnector/GpDbPostgreSql/GpDbQueryResPgSql.hpp>

#include <iostream>

namespace GPlatform {

GpDbConnectionTaskPgSql::GpDbConnectionTaskPgSql
(
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    const milliseconds_t        aConnectTimeout,
    std::string                 aServerHost,
    const u_int_16              aServerPort,
    std::string                 aUserName,
    std::string                 aPassword,
    std::string                 aDatabase
):
GpTcpClientTask
{
    GpSocketFlags{},
    aIOEventPollerIdx
},
iConnectTimeout  {aConnectTimeout},
iServerHost      {std::move(aServerHost)},
iServerPort      {aServerPort},
iMessageProcessor
{
    std::move(aUserName),
    std::move(aPassword),
    std::move(aDatabase),
    std::bind(&GpDbConnectionTaskPgSql::OnAuthOk, this)
}
{
}

GpDbConnectionTaskPgSql::~GpDbConnectionTaskPgSql (void) noexcept
{
}

GpDbConnectionTaskPgSql::ConnectedToDbFutureT::SP   GpDbConnectionTaskPgSql::GetConnectedToDbFuture (void)
{
    return iConnectedToDbPromise.Future();
}

GpDbQueryRes::SP    GpDbConnectionTaskPgSql::Execute
(
    const GpDbQuery&    aQuery,
    size_t              aMinResultRowsCount
)
{
    // Log
    LOG_INFO
    (
        fmt::format
        (
            "[GpDbConnectionTaskPgSql::Execute]: SQL '{}', values:\n{}",
            aQuery.QueryStr(),
            aQuery.Values().empty() ? "empty" : aQuery.ValuesToStr()
        )
    );

    // Create SQL request task
    ExecutePromiseT     executePromise;
    ExecuteFutureT::SP  executeFuture = executePromise.Future();
    PushMessage(ExecuteMsgT{aQuery, aMinResultRowsCount, std::move(executePromise)});

    // Wait for execute or done
    GpDbQueryResPgSql::SP res;

    // Wait for connect or done
    {
        ?????

        auto onConnectSuccessFn = [&](typename GpDbConnectionTaskPgSql::ConnectedToDbFutureT::value_type&)// OnSuccessFnT
        {
            LOG_INFO
            (
                fmt::format
                (
                    "[GpDbConnectionPgSql::TryConnectAndWaitFor]: Connected to PostgreSQL {}:{}, database '{}'",
                    iServerHost,
                    iServerPort,
                    iDatabase
                )
            );
        };

        auto onConnectExceptionFn = [&](const GpException& aEx)// OnExceptionFnT
        {
            LOG_ERROR
            (
                fmt::format
                (
                    "[GpDbConnectionPgSql::TryConnectAndWaitFor]: Failed to connect to PostgreSQL {}:{}, database '{}'",
                    iServerHost,
                    iServerPort,
                    iDatabase
                )
            );

            throw aEx;
        };

        auto onConnectFuturePack = std::tuple<GpDbConnectionTaskPgSql::ConnectedToDbFutureT&, decltype(onConnectSuccessFn), decltype(onConnectExceptionFn)>
        {
            connectedToDbFuture.Vn(),
            onConnectSuccessFn,
            onConnectExceptionFn
        };

        auto onDoneSuccessFn = [&](typename GpTask::DoneFutureT::value_type&)// OnSuccessFnT
        {
            THROW_GP
            (
                fmt::format
                (
                    "[GpDbConnectionPgSql::TryConnectAndWaitFor]: Connection task return done future while connecting to PostgreSQL {}:{}, database '{}'",
                    iServerHost,
                    iServerPort,
                    iDatabase
                )
            );
        };

        auto onDoneExceptionFn = [&](const GpException& aEx)// OnExceptionFnT
        {
            throw aEx;
        };

        auto onDoneFuturePack = std::tuple<GpTask::DoneFutureT&, decltype(onDoneSuccessFn), decltype(onDoneExceptionFn)>
        {
            doneFutureSP.Vn(),
            onDoneSuccessFn,
            onDoneExceptionFn
        };

        GpItcSharedFutureUtils::SWaitForInfAny
        (
            100.0_si_ms,
            onConnectFuturePack,
            onDoneFuturePack
        );
    }

    //GpItcSharedFutureUtils::SWaitForInf
    //(
    //  queryTaskDoneFuture.V(),
    //  [&](typename GpTaskFiber::StartFutureT::value_type& aRes)//OnSuccessFnT
    //  {
    //      LOG_INFO("[GpDbConnectionPgSql::ExecuteAsync]: OK"_sv);
    //      res = std::move(aRes.Value<GpDbQueryResPgSql::SP>());
    //  },
    //  [](const GpException& aEx)//OnExceptionFnT
    //  {
    //      throw aEx;
    //  },
    //  100.0_si_ms
    //);

    res.V().Process(aMinResultRowsCount, iPgConn);

    return res;
}

void    GpDbConnectionTaskPgSql::OnStart (void)
{
    GpTcpClientTask::OnStart();

    iProcessState = ProcessStateT::CHECK_CONNECTION;

    // TODO: implement IPv6
    // Connect
    ConnectTcpAndWait
    (
        iServerHost,
        iServerPort,
        GpSocketIPv::IPv4,
        iConnectTimeout
    );

    iSocketTmpBufferRead.resize(4096);  // TODO: move to config
    iSocketTmpBufferWrite.resize(4096); // TODO: move to config

    iProcessState = ProcessStateT::IDLE;

    // Send startup message to DB
    const size_t messageSize = iMessageProcessor.MakeStartupMessage(iSocketTmpBufferWrite);
    PrepareAndSendMessage(messageSize);
}

void    GpDbConnectionTaskPgSql::OnStop (StopExceptionsT& aStopExceptionsOut) noexcept
{
    try
    {
        LOG_INFO("[GpDbConnectionTaskPgSql::OnStop]:..."_sv);
    } catch (const GpException& ex)
    {
        aStopExceptionsOut.emplace_back(ex);
    } catch (const std::exception& ex)
    {
        aStopExceptionsOut.emplace_back(GpException{ex.what()});
    } catch (...)
    {
        aStopExceptionsOut.emplace_back(GpException{"[GpDbConnectionTaskPgSql::OnStop]: unknown exception"_sv});
    }

    GpTcpClientTask::OnStop(aStopExceptionsOut);
}

void    GpDbConnectionTaskPgSql::OnStopException (const GpException& aException) noexcept
{
    LOG_EXCEPTION
    (
        "[GpDbConnectionTaskPgSql::OnStopException]",
        aException
    );
}

void    GpDbConnectionTaskPgSql::OnReadyToRead (GpSocket& aSocket)
{
    ReadFromSocket(static_cast<GpSocketTCP&>(aSocket));
}

void    GpDbConnectionTaskPgSql::OnReadyToWrite (GpSocket& aSocket)
{
    WriteToSocket(static_cast<GpSocketTCP&>(aSocket));
}

void    GpDbConnectionTaskPgSql::OnClosed ([[maybe_unused]] GpSocket& aSocket)
{
    LOG_INFO("[GpDbConnectionTaskPgSql::OnClosed]"_sv);
}

void    GpDbConnectionTaskPgSql::OnError ([[maybe_unused]] GpSocket& aSocket)
{
    LOG_ERROR("[GpDbConnectionTaskPgSql::OnError]"_sv);
}

void    GpDbConnectionTaskPgSql::OnConnected (GpSocketTCP& aSocket)
{
    LOG_INFO
    (
        fmt::format
        (
            "[GpDbConnectionTaskPgSql::OnConnected]: connected to '{}'",
            aSocket.AddrRemote().ToStringIP()
        )
    );
}

void    GpDbConnectionTaskPgSql::ProcessOtherMessages (GpAny& aMessage)
{
#if DEBUG_BUILD
    GpDebugging::SBreakpoint();
#endif// #if DEBUG_BUILD

    LOG_ERROR
    (
        fmt::format
        (
            "[GpDbConnectionTaskPgSql::ProcessOtherMessages]: not socket message {}",
            aMessage.TypeInfo().name()
        )
    );
}

void    GpDbConnectionTaskPgSql::ReadFromSocket (GpSocketTCP& aSocket)
{
    size_t readFromSocketSize = 1;

    while (readFromSocketSize > 0)
    {
        // Read from socket
        {
            GpByteWriterStorageByteArray    writerStorage{iSocketTmpBufferRead};
            GpByteWriter                    writer{writerStorage};

            writer.SubspanThenOffsetAdd(iRsSizeOfCurrentMessage);

            readFromSocketSize = aSocket.Read(writer);

            if (readFromSocketSize == 0)
            {
                break;
            }
        }

        // Separate data stream to messages
        while (true)
        {
            GpByteReaderStorage readerStorage{iSocketTmpBufferRead};
            GpByteReader        reader{readerStorage};

            reader.OffsetAdd(1); // Message ID
            const size_t messageSize            = reader.UI32() + 1/*message id*/;// Message size
            const size_t totalReadMessageSize   = iRsSizeOfCurrentMessage + readFromSocketSize;

            if (totalReadMessageSize >= messageSize)
            {
                // Process message
                ProcessRsMessage(GpSpanByteR{std::data(iSocketTmpBufferRead), messageSize});

                // Check if there are next message
                if (totalReadMessageSize > messageSize)
                {
                    std::memmove
                    (
                        std::data(iSocketTmpBufferRead),
                        std::data(iSocketTmpBufferRead) + messageSize,
                        totalReadMessageSize - messageSize
                    );

                    iRsSizeOfCurrentMessage = totalReadMessageSize - messageSize;
                } else// totalReadMessageSize == messageSize
                {
                    iRsSizeOfCurrentMessage = 0;
                    break;
                }
            } else
            {
                break;
            }
        }
    }
}

void    GpDbConnectionTaskPgSql::WriteToSocket (GpSocketTCP& aSocket)
{
    if (iProcessState != ProcessStateT::SEND_MESSAGE) [[unlikely]]
    {
        return;
    }

    GpByteReaderStorage readerStorage{GpSpanByteR{std::data(iSocketTmpBufferWrite), iRqBytesToWriteTotal}};
    GpByteReader        reader{readerStorage};
    reader.OffsetAdd(iRqBytesWrited);

    iRqBytesWrited += aSocket.Write(reader);

    if (iRqBytesWrited >= iRqBytesToWriteTotal)
    {
        iProcessState           = ProcessStateT::IDLE;
        iRqBytesToWriteTotal    = 0;
        iRqBytesWrited          = 0;
    }
}

void    GpDbConnectionTaskPgSql::PrepareAndSendMessage (const size_t aMessageSize)
{
    // Note: all data must be stored in iSocketTmpBufferWrite

    THROW_COND_GP
    (
        iProcessState == ProcessStateT::IDLE,
        "iProcessState state must be IDLE"
    );

    iProcessState           = ProcessStateT::SEND_MESSAGE;

    // Write to socket
    iRqBytesToWriteTotal    = aMessageSize;
    iRqBytesWrited          = 0;
    WriteToSocket(SocketTCP());
}

void    GpDbConnectionTaskPgSql::ProcessRsMessage (GpSpanByteR aMessageData)
{
    const size_t outMessageSize = iMessageProcessor.ProcessRsMessage
    (
        aMessageData,
        iSocketTmpBufferWrite
    );

    if (outMessageSize > 0)
    {
        PrepareAndSendMessage(outMessageSize);
    }
}

void    GpDbConnectionTaskPgSql::OnAuthOk (void)
{
    iIsConnectd     = true;
    iProcessState   = ProcessStateT::IDLE;
    iConnectedToDbPromise.Fulfill(ConnectedToDbMsg{});
}

}// namespace GPlatform

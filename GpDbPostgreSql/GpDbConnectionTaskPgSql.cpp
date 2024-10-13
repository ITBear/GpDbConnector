#include <GpDbConnector/GpDbPostgreSql/GpDbConnectionTaskPgSql.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>
#include <GpCore2/GpUtils/Debugging/GpDebugging.hpp>
#include <GpCore2/GpTasks/ITC/GpItcSharedFutureUtils.hpp>
#include <GpDbConnector/GpDbClient/GpDbException.hpp>
#include <GpDbConnector/GpDbPostgreSql/GpDbQueryResPgSql.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>

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
    std::bind(&GpDbConnectionTaskPgSql::OnReadyForRequest, this),
    std::bind(&GpDbConnectionTaskPgSql::OnDataRow, this, std::placeholders::_1, std::placeholders::_2),
    std::bind(&GpDbConnectionTaskPgSql::OnCommandComplete, this, std::placeholders::_1),
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
    const size_t        aMinResultRowsCount
)
{
    // Log
    LOG_INFO
    (
        fmt::format
        (
            "[GpDbConnectionTaskPgSql::Execute]: SQL '{}', values: {}",
            aQuery.QueryStr(),
            aQuery.Values().empty() ? "empty" : ("\n" + aQuery.ValuesToStr())
        )
    );

    // Create SQL request task
    ExecutePromiseT     executePromise;
    ExecuteFutureT::SP  executeFuture = executePromise.Future();

    GpTaskScheduler::S().MakeTaskReady
    (
        TaskId(), ExecuteMsgT{aQuery, aMinResultRowsCount, std::move(executePromise)}
    );

    // Wait for execute or done
    GpDbQueryRes::SP res;
    {
        auto onExecuteSuccessFn = [&res](typename ExecuteFutureT::value_type& aRes)// OnSuccessFnT
        {
            res = std::move(aRes);
        };

        auto onExecuteExceptionFn = [](const GpException& aEx)// OnExceptionFnT
        {
            throw aEx;
        };

        auto onExecuteFuturePack = std::tuple<ExecuteFutureT&, decltype(onExecuteSuccessFn), decltype(onExecuteExceptionFn)>
        {
            executeFuture.Vn(),
            onExecuteSuccessFn,
            onExecuteExceptionFn
        };

        auto onDoneSuccessFn = []([[maybe_unused]] typename GpTask::DoneFutureT::value_type&)// OnSuccessFnT
        {
            THROW_GP("[GpDbConnectionTaskPgSql::Execute]: Connection task return done future while execute SQL request"_sv);
        };

        auto onDoneExceptionFn = [](const GpException& aEx)// OnExceptionFnT
        {
            throw aEx;
        };

        auto onDoneFuturePack = std::tuple<GpTask::DoneFutureT&, decltype(onDoneSuccessFn), decltype(onDoneExceptionFn)>
        {
            GetDoneFuture().Vn(),
            onDoneSuccessFn,
            onDoneExceptionFn
        };

        GpItcSharedFutureUtils::SWaitForInfAny
        (
            100.0_si_ms,
            onExecuteFuturePack,
            onDoneFuturePack
        );
    }

    return res;
}

void    GpDbConnectionTaskPgSql::OnStart (void)
{
    GpTcpClientTask::OnStart();

    iSocketState = SocketStateT::CONNECTING;

    // TODO: implement IPv6
    // Connect
    ConnectTcpAndWait
    (
        iServerHost,
        iServerPort,
        GpSocketIPv::IPv4,
        iConnectTimeout
    );

    iSocketState = SocketStateT::IDLE;

    iSocketTmpBufferRead.resize(4096);  // TODO: move to config
    iSocketTmpBufferWrite.resize(4096); // TODO: move to config

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
    THROW_COND_GP
    (
        aMessage.IsContatinType<ExecuteMsgT>() == true,
        [&aMessage]()
        {
            return fmt::format
            (
                "Unsupported message type '{}'",
                aMessage.TypeInfo().name()
            );
        }
    );

    ProcessExecuteMsg(aMessage.ValueNoCheck<ExecuteMsgT>());
}

void    GpDbConnectionTaskPgSql::ProcessExecuteMsg (ExecuteMsgT& aMessage)
{
    try
    {
        // Get message values
        auto& [dbQuery, minResultRowsCount, executePromise] = aMessage;

        iExecutePromise = std::move(executePromise);

        // Check `iSocketState`
        THROW_COND_GP
        (
            iSocketState == SocketStateT::IDLE,
            [&]()
            {
                return fmt::format
                (
                    "Socket state expected to be IDLE (1), but actual state was is {}",
                    int(iSocketState)
                );
            }
        );

        // DB query to PostgreSQL message
        if (!dbQuery.Values().empty()) [[likely]]
        {
            //?
            // TODO: implement
        } else
        {
            //?
            // Send simple SQL query (no values)
            const size_t messageSize = iMessageProcessor.MakeQueryMessage(iSocketTmpBufferWrite, dbQuery.QueryStr());
            PrepareAndSendMessage(messageSize);
        }
    } catch (const GpException& ex)
    {
        iExecutePromise.Fulfill(ex);
    } catch (const std::exception& ex)
    {
        iExecutePromise.Fulfill(GpException{ex.what()});
    } catch (...)
    {
        iExecutePromise.Fulfill(GpException{"Unknown exception"});
    }

    // GpDbQueryResPgSql::SP res = MakeSP<GpDbQueryResPgSql>();
    // executePromise.Fulfill(std::move(res));
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

            writer.SubspanThenOffsetAdd(iSocketTmpBufferReadSize);

            readFromSocketSize = aSocket.Read(writer);

            if (readFromSocketSize == 0)
            {
                break;
            }

            iSocketTmpBufferReadSize += readFromSocketSize;
        }

        // Separate data stream to messages
        while (true)
        {
            GpByteReaderStorage readerStorage{iSocketTmpBufferRead};
            GpByteReader        reader{readerStorage};

            reader.OffsetAdd(1); // Message ID
            const size_t messageSize = reader.UI32() + 1/*message id*/;// Message size

            if (iSocketTmpBufferReadSize < messageSize) [[unlikely]]
            {
                break;
            }

            // Process message
            ProcessRsMessage(GpSpanByteR{std::data(iSocketTmpBufferRead), messageSize});

            // Check if there are next message
            if (iSocketTmpBufferReadSize == messageSize) [[likely]]
            {
                iSocketTmpBufferReadSize = 0;
                break;
            }

            iSocketTmpBufferReadSize -= messageSize;

            std::memmove
            (
                std::data(iSocketTmpBufferRead),
                std::data(iSocketTmpBufferRead) + messageSize,
                iSocketTmpBufferReadSize
            );
        }
    }
}

void    GpDbConnectionTaskPgSql::WriteToSocket (GpSocketTCP& aSocket)
{
    if (iSocketState != SocketStateT::SENDING_MESSAGE) [[unlikely]]
    {
        return;
    }

    GpByteReaderStorage readerStorage{GpSpanByteR{std::data(iSocketTmpBufferWrite), iRqBytesToWriteTotal}};
    GpByteReader        reader{readerStorage};

    if (iRqBytesWrited > 0)
    {
        reader.OffsetAdd(iRqBytesWrited);
    }

    iRqBytesWrited += aSocket.Write(reader);

    if (iRqBytesWrited >= iRqBytesToWriteTotal)
    {
        iSocketState            = SocketStateT::IDLE;
        iRqBytesToWriteTotal    = 0;
        iRqBytesWrited          = 0;
    }
}

void    GpDbConnectionTaskPgSql::PrepareAndSendMessage (const size_t aMessageSize)
{
    // Note: all data must be stored in iSocketTmpBufferWrite

    THROW_COND_GP
    (
        iSocketState == SocketStateT::IDLE,
        "iSocketState state must be IDLE"
    );

    iSocketState            = SocketStateT::SENDING_MESSAGE;

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

void    GpDbConnectionTaskPgSql::OnReadyForRequest (void)
{
    if (iIsConnectd == false) [[unlikely]]
    {
        iIsConnectd = true;
        iConnectedToDbPromise.Fulfill(ConnectedToDbMsg{});
    }
}

void    GpDbConnectionTaskPgSql::OnDataRow
(
    const PSQL::RowDescriptionDescRS&   aRowDesc,
    const PSQL::DataRowDescRS&          aRowData
)
{
    if (iDbQueryResSP.IsNULL())
    {
        iDbQueryResSP = MakeSP<GpDbQueryResPgSql>();
    }

    iDbQueryResSP.Vn().AddDataRow(aRowDesc, aRowData);
}

void    GpDbConnectionTaskPgSql::OnCommandComplete ([[maybe_unused]] const PSQL::CommandCompleteDescRS& aCommandCompleteDesc)
{
    iExecutePromise.Fulfill(std::move(iDbQueryResSP));
}

}// namespace GPlatform

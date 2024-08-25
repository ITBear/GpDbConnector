#include <GpDbConnector/GpDbClient/GpDbConnectionGuard.hpp>
#include <GpDbConnector/GpDbClient/GpDbManager.hpp>
#include <GpDbConnector/GpDbClient/GpDbManagerCatalog.hpp>
#include <GpDbConnector/GpDbClient/GpDbConnection.hpp>
#include <GpDbConnector/GpDbClient/GpDbException.hpp>

#include <GpLog/GpLogCore/GpLog.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp>
#include <GpCore2/GpTasks/GpTask.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiberCtxForceUnwind.hpp>

namespace GPlatform {

GpDbConnectionGuard::GpDbConnectionGuard (GpDbManager& aManager) noexcept:
iManager{aManager}
{
}

GpDbConnectionGuard::GpDbConnectionGuard (std::string_view aManagerName):
GpDbConnectionGuard{GpDbManagerCatalog::S().Find(aManagerName)}
{
}

GpDbConnectionGuard::~GpDbConnectionGuard (void) noexcept
{
    try
    {
        ConnectionRelease();
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr("[GpDbConnectionGuard::~GpDbConnectionGuard]: exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr("[GpDbConnectionGuard::~GpDbConnectionGuard]: exception: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr("[GpDbConnectionGuard::~GpDbConnectionGuard]: unknown exception"_sv);
    }
}

void    GpDbConnectionGuard::BeginTransaction (const GpDbTransactionIsolation::EnumT aIsolationLevel)
{
    ConnectionAcquire().BeginTransaction(aIsolationLevel);
}

void    GpDbConnectionGuard::CommitTransaction (void)
{
    GpDbConnection& connection = ConnectionAcquire();

    std::optional<GpException> exOpt;

    try
    {
        connection.CommitTransaction();
    } catch (const GpException& ex)
    {
        exOpt = ex;
    } catch (const std::exception& ex)
    {
        exOpt = GpException{ex.what()};
    } catch (...)
    {
        exOpt = GpException{"[GpDbConnectionGuard::CommitTransaction]: Unknown exception"};
    }

    ConnectionRelease();

    if (exOpt.has_value())
    {
        throw exOpt.value();
    }
}

void    GpDbConnectionGuard::RollbackTransaction (void)
{
    GpDbConnection& connection = ConnectionAcquire();

    std::optional<GpException> exOpt;

    try
    {
        connection.RollbackTransaction();
    } catch (const GpException& ex)
    {
        exOpt = ex;
    } catch (const std::exception& ex)
    {
        exOpt = GpException{ex.what()};
    } catch (...)
    {
        exOpt = GpException{"[GpDbConnectionGuard::RollbackTransaction]: Unknown exception"};
    }

    ConnectionRelease();

    if (exOpt.has_value())
    {
        throw exOpt.value();
    }
}

GpDbQueryRes::SP    GpDbConnectionGuard::Execute
(
    const GpDbQuery&    aQuery,
    const size_t        aMinResultRowsCount
)
{
    GpDbConnection&     connection = ConnectionAcquire();
    GpDbQueryRes::SP    res;

    std::optional<GpException> exOpt;

    try
    {
        res = connection.Execute(aQuery, aMinResultRowsCount);
    } catch (const GpException& ex)
    {
        exOpt = ex;
    } catch (const std::exception& ex)
    {
        exOpt = GpException{ex.what()};
    } catch (...)
    {
        exOpt = GpException{"[GpDbConnectionGuard::Execute]: Unknown exception"};
    }

    if (   exOpt.has_value()
        || (connection.IsTransactionOpen() == false))
    {
        ConnectionRelease();

        if (exOpt.has_value())
        {
            throw exOpt.value();
        }
    }

    return res;
}

GpDbQueryRes::SP    GpDbConnectionGuard::Execute
(
    std::string_view    aSQL,
    const size_t        aMinResultRowsCount
)
{
    GpDbQuery query(aSQL);
    return Execute(query, aMinResultRowsCount);
}

GpDbConnection& GpDbConnectionGuard::ConnectionAcquire (void)
{
    if (iConnection.IsNULL())
    {
        auto res = Manager().Acquire();

        THROW_COND_DB
        (
            res.has_value(),
            GpDbExceptionCode::CONNECTION_LIMIT_EXCEEDED,
            "DB connection limit exceeded"_sv
        );

        iConnection = res.value();
    }

    return iConnection.Vn();
}

void    GpDbConnectionGuard::ConnectionRelease (void)
{
    if (iConnection.IsNULL())
    {
        return;     
    }

    GpDbConnection& conn = iConnection.Vn();

    if (conn.IsTransactionOpen())
    {
        try
        {           
            conn.RollbackTransaction();
        } catch (GpTaskFiberCtxForceUnwind&)
        {
            iConnection.Clear();
            throw;
        } catch (const GpException& ex)
        {
            const auto currentTaskOpt = GpTask::SCurrentTask();

            if (currentTaskOpt.has_value())
            {
                LOG_EXCEPTION("[GpDbConnectionGuard::ConnectionRelease]", ex, currentTaskOpt.value().get().TaskIdAsUUID());
            } else
            {
                LOG_EXCEPTION("[GpDbConnectionGuard::ConnectionRelease]", ex);
            }
        } catch (const std::exception& e)
        {
            const auto currentTaskOpt = GpTask::SCurrentTask();

            if (currentTaskOpt.has_value())
            {
                LOG_EXCEPTION("[GpDbConnectionGuard::ConnectionRelease]", GpException{e.what()}, currentTaskOpt.value().get().TaskIdAsUUID());
            } else
            {
                LOG_EXCEPTION("[GpDbConnectionGuard::ConnectionRelease]", GpException{e.what()});
            }
        } catch (...)
        {
            const auto currentTaskOpt = GpTask::SCurrentTask();

            if (currentTaskOpt.has_value())
            {
                LOG_EXCEPTION("[GpDbConnectionGuard::ConnectionRelease]", GpException{"Unknonwn excpeption"}, currentTaskOpt.value().get().TaskIdAsUUID());
            } else
            {
                LOG_EXCEPTION("[GpDbConnectionGuard::ConnectionRelease]", GpException{"Unknonwn excpeption"});
            }
        }
    }

    Manager().Release(std::move(iConnection));
    iConnection.Clear();
}

}// namespace GPlatform

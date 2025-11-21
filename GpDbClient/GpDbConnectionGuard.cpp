#include <GpDbConnector/GpDbClient/GpDbConnectionGuard.hpp>
#include <GpDbConnector/GpDbClient/GpDbManager.hpp>
#include <GpDbConnector/GpDbClient/GpDbManagerCatalog.hpp>
#include <GpDbConnector/GpDbClient/GpDbConnection.hpp>
#include <GpDbConnector/GpDbClient/GpDbException.hpp>
#include <GpDbConnector/GpDbClient/Query/GpDbQueryBuilder.hpp>

#include <GpLog/GpLogCore/GpLog.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpOutUtils.hpp>
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
        GpOutUtils::S().Err("[GpDbConnectionGuard::~GpDbConnectionGuard]: exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpOutUtils::S().Err("[GpDbConnectionGuard::~GpDbConnectionGuard]: exception: "_sv + e.what());
    } catch (...)
    {
        GpOutUtils::S().Err("[GpDbConnectionGuard::~GpDbConnectionGuard]: unknown exception"_sv);
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

GpDbQueryRes::SP    GpDbConnectionGuard::Execute (const GpDbQuery& aQuery)
{
    GpDbConnection&     connection = ConnectionAcquire();
    GpDbQueryRes::SP    res;

    std::optional<GpException> exOpt;

    try
    {
        res = connection.Execute(aQuery);
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

GpDbConnection& GpDbConnectionGuard::ConnectionAcquire (void)
{
    if (iConnection.IsNULL())
    {
        auto res = Manager().Acquire();

        VERIFY
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
            GpTask::WP taskWP = GpTask::SCurrentTask();

            if (GpTask::SP taskSP = taskWP.Lock(); taskSP.IsNotNULL())
            {
                LOG_EXCEPTION("[GpDbConnectionGuard::ConnectionRelease]", ex, taskSP.Vn().TaskIdAsUUID());
            } else
            {
                LOG_EXCEPTION("[GpDbConnectionGuard::ConnectionRelease]", ex);
            }
        } catch (const std::exception& e)
        {
            GpTask::WP taskWP = GpTask::SCurrentTask();

            if (GpTask::SP taskSP = taskWP.Lock(); taskSP.IsNotNULL())
            {
                LOG_EXCEPTION("[GpDbConnectionGuard::ConnectionRelease]", GpException{e.what()}, taskSP.Vn().TaskIdAsUUID());
            } else
            {
                LOG_EXCEPTION("[GpDbConnectionGuard::ConnectionRelease]", GpException{e.what()});
            }
        } catch (...)
        {
            GpTask::WP taskWP = GpTask::SCurrentTask();

            if (GpTask::SP taskSP = taskWP.Lock(); taskSP.IsNotNULL())
            {
                LOG_EXCEPTION("[GpDbConnectionGuard::ConnectionRelease]", GpException{"Unknonwn excpeption"}, taskSP.Vn().TaskIdAsUUID());
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

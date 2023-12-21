#include "GpDbConnectionGuard.hpp"
#include "GpDbManager.hpp"
#include "GpDbConnection.hpp"
#include "GpDbException.hpp"

#include <GpLog/GpLogCore/GpLog.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp>
#include <GpCore2/GpTasks/GpTask.hpp>

GP_WARNING_PUSH()
GP_WARNING_DISABLE(shadow)

#include <boost/context/fiber.hpp>

GP_WARNING_POP()

namespace GPlatform {

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
    } catch (const boost::context::detail::forced_unwind&)
    {
        GpStringUtils::SCerr("[GpDbConnectionGuard::~GpDbConnectionGuard]: exception: boost::context::detail::forced_unwind"_sv);
    } catch (...)
    {
        GpStringUtils::SCerr("[GpDbConnectionGuard::~GpDbConnectionGuard]: unknown exception"_sv);
    }
}

void    GpDbConnectionGuard::CommitTransaction (void)
{
    GpDbConnection& connection = ConnectionAcquire();

    std::optional<std::exception_ptr> eptr;

    try
    {
        connection.CommitTransaction();
    } catch (...)
    {
        eptr = std::current_exception();
    }

    ConnectionRelease();

    if (eptr.has_value())
    {
        std::rethrow_exception(eptr.value());
    }
}

void    GpDbConnectionGuard::RollbackTransaction (void)
{
    GpDbConnection& connection = ConnectionAcquire();

    std::optional<std::exception_ptr> eptr;

    try
    {
        connection.RollbackTransaction();
    } catch (...)
    {
        eptr = std::current_exception();
    }

    ConnectionRelease();

    if (eptr.has_value())
    {
        std::rethrow_exception(eptr.value());
    }
}

GpDbQueryRes::SP    GpDbConnectionGuard::Execute
(
    const GpDbQuery&    aQuery,
    const size_t        aMinResultRowsCount
)
{
    GpDbQueryPrepared::CSP  queryPreparedCSP    = Manager().Prepare(aQuery);
    GpDbConnection&         connection          = ConnectionAcquire();
    GpDbQueryRes::SP        res;

    std::optional<std::exception_ptr> eptr;

    try
    {
        res = connection.Execute(aQuery, queryPreparedCSP.V(), aMinResultRowsCount);
    } catch (...)
    {
        eptr = std::current_exception();
    }

    if (   eptr.has_value()
        || (connection.IsTransactionOpen() == false))
    {
        ConnectionRelease();

        if (eptr.has_value())
        {
            std::rethrow_exception(eptr.value());
        }
    }

    return res;
}

GpDbQueryRes::SP    GpDbConnectionGuard::Execute
(
    std::u8string_view  aSQL,
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
        } catch (const std::exception& e)
        {
            const auto currentTaskOpt = GpTask::SCurrentTask();

            if (currentTaskOpt.has_value())
            {
                LOG_EXCEPTION(e, currentTaskOpt.value().get().IdAsUUID());
            } else
            {
                LOG_EXCEPTION(e);
            }
        } catch (const boost::context::detail::forced_unwind&)
        {
            iConnection.Clear();
            throw;
        } catch (...)
        {
            const auto currentTaskOpt = GpTask::SCurrentTask();

            if (currentTaskOpt.has_value())
            {
                LOG_EXCEPTION(currentTaskOpt.value().get().IdAsUUID());
            } else
            {
                LOG_EXCEPTION();
            }
        }
    }

    Manager().Release(std::move(iConnection));
}

}//namespace GPlatform

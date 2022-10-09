#include "GpDbConnectionGuard.hpp"
#include "GpDbManager.hpp"
#include "GpDbConnection.hpp"
#include "GpDbManagerCatalog.hpp"
#include "GpDbException.hpp"

namespace GPlatform {

GpDbConnectionGuard::GpDbConnectionGuard (GpDbManager& aManager) noexcept:
iManager(aManager)
{
}

GpDbConnectionGuard::~GpDbConnectionGuard (void) noexcept
{
    ConnectionRelease();
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
    std::string_view    aSQL,
    const size_t        aMinResultRowsCount
)
{
    GpDbQuery query((std::string(aSQL)));
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

void    GpDbConnectionGuard::ConnectionRelease (void) noexcept
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
            Manager().Release(iConnection);
        } catch (const std::exception& e)
        {
            LOG_EXCEPTION(e, GpTask::SCurrentUID());
        } catch (...)
        {
            LOG_EXCEPTION(GpTask::SCurrentUID());
        }
    } else
    {
        Manager().Release(iConnection);
    }

    iConnection.Clear();
}

}//namespace GPlatform

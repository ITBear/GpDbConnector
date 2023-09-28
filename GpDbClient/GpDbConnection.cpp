#include "GpDbConnection.hpp"
#include "GpDbConnectionHookManager.hpp"
#include "../../GpCore2/GpUtils/Other/GpRAIIonDestruct.hpp"

namespace GPlatform {

GpDbConnection::~GpDbConnection (void) noexcept
{
}

void    GpDbConnection::BeginTransaction (GpDbTransactionIsolation::EnumT aIsolationLevel)
{
    THROW_COND_GP
    (
        iIsTransactionOpen == false,
        u8"Transaction already open"_sv
    );

    OnBeginTransaction(aIsolationLevel);

    iIsTransactionOpen  = true;
    iTransactionLevel   = aIsolationLevel;
}

void    GpDbConnection::CommitTransaction (void)
{
    THROW_COND_GP
    (
        iIsTransactionOpen == true,
        u8"Transaction not open"_sv
    );

    try
    {
        OnCommitTransaction();
    } catch (...)
    {
        iIsTransactionOpen  = false;
        iTransactionLevel   = TransactionLevelTE::READ_UNCOMMITTED;

        throw;
    }

    iIsTransactionOpen  = false;
    iTransactionLevel   = TransactionLevelTE::READ_UNCOMMITTED;
}

void    GpDbConnection::RollbackTransaction (void)
{
    THROW_COND_GP
    (
        iIsTransactionOpen == true,
        "Transaction not open"_sv
    );

    try
    {
        OnRollbackTransaction();
    } catch (...)
    {
        iIsTransactionOpen  = false;
        iTransactionLevel   = TransactionLevelTE::READ_UNCOMMITTED;

        throw;
    }

    iIsTransactionOpen  = false;
    iTransactionLevel   = TransactionLevelTE::READ_UNCOMMITTED;
}

GpDbQueryRes::SP    GpDbConnection::Execute
(
    const GpDbQuery&            aQuery,
    const GpDbQueryPrepared&    aQueryPrepared,
    const size_t                aMinResultRowsCount
)
{
    const bool _isCallHook = iIsCallHook;

    GpRAIIonDestruct isCallHookRestore
    (
        [&]()
        {
            iIsCallHook = _isCallHook;
        }
    );

    if (_isCallHook)
    {
        iIsCallHook = false;
        GpDbConnectionHookManager::S().OnExecute
        (
            *this,
            GpDbConnectionHookManager::Mode::BEFORE_EXECUTE
        );
    }

    GpDbQueryRes::SP res = _Execute
    (
        aQuery,
        aQueryPrepared,
        aMinResultRowsCount
    );

    if (_isCallHook)
    {
        iIsCallHook = false;
        GpDbConnectionHookManager::S().OnExecute
        (
            *this,
            GpDbConnectionHookManager::Mode::AFTER_EXECUTE
        );
    }

    return res;
}

}//namespace GPlatform

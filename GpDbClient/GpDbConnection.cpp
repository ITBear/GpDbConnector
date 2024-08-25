#include <GpDbConnector/GpDbClient/GpDbConnection.hpp>

namespace GPlatform {

GpDbConnection::GpDbConnection (void) noexcept
{
}

GpDbConnection::~GpDbConnection (void) noexcept
{
}

void    GpDbConnection::BeginTransaction (GpDbTransactionIsolation::EnumT aIsolationLevel)
{
    THROW_COND_GP
    (
        iIsTransactionOpen == false,
        "Transaction already open"_sv
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
        "Transaction not open"_sv
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

}// namespace GPlatform

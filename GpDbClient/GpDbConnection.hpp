#pragma once

#include <GpDbConnector/GpDbClient/GpDbTransactionIsolation.hpp>
#include <GpDbConnector/GpDbClient/Query/GpDbQuery.hpp>
#include <GpDbConnector/GpDbClient/Query/GpDbQueryRes.hpp>
#include <GpDbConnector/GpDbClient/Query/GpDbQueryPrepared.hpp>

namespace GPlatform {

class GP_DB_CLIENT_API GpDbConnection
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpDbConnection)
    CLASS_DD(GpDbConnection)

    using TransactionLevelTE = GpDbTransactionIsolation::EnumT;

protected:
                                GpDbConnection          (void) noexcept;

public:
    virtual                     ~GpDbConnection         (void) noexcept;

    bool                        IsTransactionOpen       (void) const noexcept {return iIsTransactionOpen;}
    void                        BeginTransaction        (GpDbTransactionIsolation::EnumT aIsolationLevel);
    void                        CommitTransaction       (void);
    void                        RollbackTransaction     (void);
    TransactionLevelTE          TransactionLevel        (void) const noexcept {return iTransactionLevel;}

    virtual void                Close                   (void) = 0;
    virtual GpDbQueryRes::SP    Execute                 (const GpDbQuery&   aQuery,
                                                         size_t             aMinResultRowsCount) = 0;

    virtual bool                IsConnected             (void) const noexcept = 0;
    virtual void                TryConnectAndWaitFor    (void) = 0;

protected:
    virtual void                OnBeginTransaction      (GpDbTransactionIsolation::EnumT aIsolationLevel) = 0;
    virtual void                OnCommitTransaction     (void) = 0;
    virtual void                OnRollbackTransaction   (void) = 0;

private:
    TransactionLevelTE          iTransactionLevel   = TransactionLevelTE::READ_UNCOMMITTED;
    bool                        iIsTransactionOpen  = false;
};

}// namespace GPlatform

#pragma once

#include "GpDbConnectionStatus.hpp"
#include "GpDbConnectionMode.hpp"
#include "GpDbTransactionIsolation.hpp"
#include "Query/GpDbQuery.hpp"
#include "Query/GpDbQueryRes.hpp"
#include "Query/GpDbQueryPrepared.hpp"

namespace GPlatform {

class GP_DB_CLIENT_API GpDbConnection
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbConnection)
    CLASS_DD(GpDbConnection)

    using StatusTE              = GpDbConnectionStatus::EnumT;
    using ModeTE                = GpDbConnectionMode::EnumT;
    using TransactionLevelTE    = GpDbTransactionIsolation::EnumT;
    using PreExecuteHookFnT     = std::function<void(GpDbConnection& aConn)>;

protected:
    inline                      GpDbConnection          (StatusTE   aStatus,
                                                         ModeTE     aMode) noexcept;

public:
    virtual                     ~GpDbConnection         (void) noexcept;

    StatusTE                    Status                  (void) const noexcept {return iStatus;}
    ModeTE                      Mode                    (void) const noexcept {return iMode;}

    bool                        IsTransactionOpen       (void) const noexcept {return iIsTransactionOpen;}
    void                        BeginTransaction        (GpDbTransactionIsolation::EnumT aIsolationLevel);
    void                        CommitTransaction       (void);
    void                        RollbackTransaction     (void);
    TransactionLevelTE          TransactionLevel        (void) const noexcept {return iTransactionLevel;}

    virtual void                Close                   (void) = 0;
    GpDbQueryRes::SP            Execute                 (const GpDbQuery&           aQuery,
                                                         const GpDbQueryPrepared&   aQueryPrepared,
                                                         size_t                     aMinResultRowsCount);

    virtual std::string         StrEscape               (std::string_view aStr) const = 0;
    virtual bool                Validate                (void) const noexcept = 0;
    virtual void                SetEnv                  (const std::vector<std::tuple<std::string, std::string>>& aValues) = 0;

protected:
    virtual GpDbQueryRes::SP    _Execute                (const GpDbQuery&           aQuery,
                                                         const GpDbQueryPrepared&   aQueryPrepared,
                                                         size_t                     aMinResultRowsCount) = 0;

    void                        SetStatus               (StatusTE aStatus) noexcept {iStatus = aStatus;}

    virtual void                OnBeginTransaction      (GpDbTransactionIsolation::EnumT aIsolationLevel) = 0;
    virtual void                OnCommitTransaction     (void) = 0;
    virtual void                OnRollbackTransaction   (void) = 0;

private:
    StatusTE                    iStatus             = StatusTE::CLOSED;
    const ModeTE                iMode;
    bool                        iIsTransactionOpen  = false;
    TransactionLevelTE          iTransactionLevel   = TransactionLevelTE::READ_UNCOMMITTED;
    bool                        iIsCallHook         = true;
};

GpDbConnection::GpDbConnection
(
    const StatusTE  aStatus,
    const ModeTE    aMode
) noexcept:
iStatus{aStatus},
iMode  {aMode}
{
}

}// namespace GPlatform

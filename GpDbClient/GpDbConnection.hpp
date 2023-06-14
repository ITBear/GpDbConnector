#pragma once

#include "GpDbConnectionStatus.hpp"
#include "GpDbConnectionMode.hpp"
#include "GpDbTransactionIsolation.hpp"
#include "Query/GpDbQuery.hpp"
#include "Query/GpDbQueryRes.hpp"
#include "Query/GpDbQueryPrepared.hpp"
#include "../../GpNetwork/GpNetworkCore/IO/Events/GpIOEventPoller.hpp"

namespace GPlatform {

class GP_DB_CLIENT_API GpDbConnection
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbConnection)
    CLASS_DD(GpDbConnection)

    using StatusTE              = GpDbConnectionStatus::EnumT;
    using ModeTE                = GpDbConnectionMode::EnumT;
    using TransactionLevelTE    = GpDbTransactionIsolation::EnumT;

protected:
    inline                          GpDbConnection          (const StatusTE         aStatus,
                                                             const ModeTE           aMode,
                                                             GpIOEventPoller::SP    aEventPoller) noexcept;
    inline                          GpDbConnection          (const StatusTE         aStatus,
                                                             const ModeTE           aMode) noexcept;

public:
    virtual                         ~GpDbConnection         (void) noexcept;

    GpIOEventPoller::SP             EventPoller             (void) const noexcept {return iEventPoller;}
    StatusTE                        Status                  (void) const noexcept {return iStatus;}
    ModeTE                          Mode                    (void) const noexcept {return iMode;}

    bool                            IsTransactionOpen       (void) const noexcept {return iIsTransactionOpen;}
    void                            BeginTransaction        (GpDbTransactionIsolation::EnumT aIsolationLevel);
    void                            CommitTransaction       (void);
    void                            RollbackTransaction     (void);
    TransactionLevelTE              TransactionLevel        (void) const noexcept {return iTransactionLevel;}

    virtual void                    Close                   (void) = 0;
    virtual GpDbQueryRes::SP        Execute                 (const GpDbQuery&           aQuery,
                                                             const GpDbQueryPrepared&   aQueryPrepared,
                                                             const size_t               aMinResultRowsCount) = 0;

    virtual std::u8string           StrEscape               (std::u8string_view aStr) const = 0;

    virtual bool                    Validate                (void) const noexcept = 0;

protected:
    void                            SetStatus               (StatusTE aStatus) noexcept {iStatus = aStatus;}

    virtual void                    OnBeginTransaction      (GpDbTransactionIsolation::EnumT aIsolationLevel) = 0;
    virtual void                    OnCommitTransaction     (void) = 0;
    virtual void                    OnRollbackTransaction   (void) = 0;

private:
    StatusTE                        iStatus             = StatusTE::CLOSED;
    const ModeTE                    iMode;
    bool                            iIsTransactionOpen  = false;
    TransactionLevelTE              iTransactionLevel   = TransactionLevelTE::READ_UNCOMMITTED;
    GpIOEventPoller::SP             iEventPoller;
};

GpDbConnection::GpDbConnection
(
    const StatusTE      aStatus,
    const ModeTE        aMode,
    GpIOEventPoller::SP aEventPoller
) noexcept:
iStatus     (aStatus),
iMode       (aMode),
iEventPoller(std::move(aEventPoller))
{
}

GpDbConnection::GpDbConnection
(
    const StatusTE  aStatus,
    const ModeTE    aMode
) noexcept:
iStatus(aStatus),
iMode  (aMode)
{
}

}//namespace GPlatform

#pragma once

#include "GpDbConnection.hpp"

namespace GPlatform {

class GpDbManager;

class GP_DB_CLIENT_API GpDbConnectionGuard
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbConnectionGuard)
    CLASS_DD(GpDbConnectionGuard)

    using ManagerRefT = std::optional<std::reference_wrapper<GpDbManager>>;

public:
                                GpDbConnectionGuard     (GpDbManager& aManager) noexcept;
                                ~GpDbConnectionGuard    (void) noexcept;

    inline void                 BeginTransaction        (GpDbTransactionIsolation::EnumT aIsolationLevel);
    void                        CommitTransaction       (void);
    void                        RollbackTransaction     (void);

    virtual GpDbQueryRes::SP    Execute                 (const GpDbQuery&   aQuery,
                                                         const size_t       aMinResultRowsCount);
    virtual GpDbQueryRes::SP    Execute                 (std::u8string_view aSQL,
                                                         const size_t       aMinResultRowsCount);

private:
    GpDbManager&                Manager                 (void) {return iManager;}
    GpDbConnection&             ConnectionAcquire       (void);
    void                        ConnectionRelease       (void);

private:
    GpDbManager&                iManager;
    GpDbConnection::SP          iConnection;
};

void    GpDbConnectionGuard::BeginTransaction (GpDbTransactionIsolation::EnumT aIsolationLevel)
{
    ConnectionAcquire().BeginTransaction(aIsolationLevel);
}

}//GPlatform

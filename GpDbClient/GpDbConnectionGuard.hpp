#pragma once

#include "GpDbManagerCatalog.hpp"
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
    inline                      GpDbConnectionGuard     (GpDbManager& aManager) noexcept;
    inline                      GpDbConnectionGuard     (std::string_view aManagerName);
                                ~GpDbConnectionGuard    (void) noexcept;

    inline void                 BeginTransaction        (GpDbTransactionIsolation::EnumT aIsolationLevel);
    void                        CommitTransaction       (void);
    void                        RollbackTransaction     (void);

    virtual GpDbQueryRes::SP    Execute                 (const GpDbQuery&   aQuery,
                                                         const size_t       aMinResultRowsCount);
    virtual GpDbQueryRes::SP    Execute                 (std::string_view   aSQL,
                                                         const size_t       aMinResultRowsCount);

private:
    GpDbManager&                Manager                 (void) {return iManager;}
    GpDbConnection&             ConnectionAcquire       (void);
    void                        ConnectionRelease       (void);

private:
    GpDbManager&                iManager;
    GpDbConnection::SP          iConnection;
};

GpDbConnectionGuard::GpDbConnectionGuard (GpDbManager& aManager) noexcept:
iManager{aManager}
{
}

GpDbConnectionGuard::GpDbConnectionGuard (std::string_view aManagerName):
GpDbConnectionGuard(GpDbManagerCatalog::S().Find(aManagerName))
{
}

void    GpDbConnectionGuard::BeginTransaction (GpDbTransactionIsolation::EnumT aIsolationLevel)
{
    ConnectionAcquire().BeginTransaction(aIsolationLevel);
}

}// namespace GPlatform

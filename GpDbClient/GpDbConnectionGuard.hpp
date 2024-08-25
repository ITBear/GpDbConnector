#pragma once

#include <GpDbConnector/GpDbClient/GpDbConnection.hpp>

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
                                GpDbConnectionGuard     (std::string_view aManagerName);
    virtual                     ~GpDbConnectionGuard    (void) noexcept;

    void                        BeginTransaction        (GpDbTransactionIsolation::EnumT aIsolationLevel);
    void                        CommitTransaction       (void);
    void                        RollbackTransaction     (void);

    virtual GpDbQueryRes::SP    Execute                 (const GpDbQuery&   aQuery,
                                                         size_t             aMinResultRowsCount);
    virtual GpDbQueryRes::SP    Execute                 (std::string_view   aSQL,
                                                         size_t             aMinResultRowsCount);

private:
    GpDbManager&                Manager                 (void) {return iManager;}
    GpDbConnection&             ConnectionAcquire       (void);
    void                        ConnectionRelease       (void);

private:
    GpDbManager&                iManager;
    GpDbConnection::SP          iConnection;
};

}// namespace GPlatform

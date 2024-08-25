#pragma once

#include <GpDbConnector/GpDbClient/GpDbConnection.hpp>
#include <GpDbConnector/GpDbPostgreSql/GpDbConnectionTaskPgSql.hpp>

namespace GPlatform {

class GpDbConnectionPgSql final: public GpDbConnection
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbConnectionPgSql)
    CLASS_DD(GpDbConnectionPgSql)

    using IsolationLevelNamesT  = std::array<std::string_view, GpDbTransactionIsolation::SCount()>;

public:
                                GpDbConnectionPgSql     (GpIOEventPollerIdx aIOEventPollerIdx,
                                                         milliseconds_t     aConnectTimeout,
                                                         std::string        aServerHost,
                                                         u_int_16           aServerPort,
                                                         std::string        aUserName,
                                                         std::string        aPassword,
                                                         std::string        aDatabase);
    virtual                     ~GpDbConnectionPgSql    (void) noexcept override final;

    virtual void                Close                   (void) override final;
    virtual GpDbQueryRes::SP    Execute                 (const GpDbQuery&   aQuery,
                                                         size_t             aMinResultRowsCount) override final;
    virtual bool                IsConnected             (void) const noexcept override final;
    virtual void                TryConnectAndWaitFor    (void) override final;

protected:
    virtual void                OnBeginTransaction      (GpDbTransactionIsolation::EnumT aIsolationLevel) override final;
    virtual void                OnCommitTransaction     (void) override final;
    virtual void                OnRollbackTransaction   (void) override final;

private:
    void                        _Close                  (void) noexcept;

private:
    const GpIOEventPollerIdx    iIOEventPollerIdx;
    const milliseconds_t        iConnectTimeout;
    const std::string           iServerHost;
    const u_int_16              iServerPort;
    const std::string           iUserName;
    const std::string           iPassword;
    const std::string           iDatabase;

    GpDbConnectionTaskPgSql::SP iConnectionTaskSP;

    static const IsolationLevelNamesT   sIsolationLevelNames;
};

}// namespace GPlatform

#pragma once

#include <GpDbConnector/GpDbClient/GpDbConnection.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPoller.hpp>
#include <postgresql/libpq-fe.h>

namespace GPlatform {

enum class GpPosrgresQueryResultType
{
    TEXT    = 0,
    BINARY  = 1
};

class GpDbConnectionPgSql final: public GpDbConnection
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbConnectionPgSql)
    CLASS_DD(GpDbConnectionPgSql)

    using IsolationLevelNamesT = std::array<std::string_view, GpDbTransactionIsolation::SCount()>;

public:
    inline                      GpDbConnectionPgSql     (PGconn*            aPgConn,
                                                         const ModeTE       aMode,
                                                         GpIOEventPollerIdx aIOEventPollerIdx) noexcept;
    virtual                     ~GpDbConnectionPgSql    (void) noexcept override final;

    GpSocketId                  SocketId                (void) const {return PQsocket(iPgConn);}
    PGconn*                     PgConn                  (void) noexcept {return iPgConn;}
    GpIOEventPollerIdx          IOEventPollerIdx        (void) noexcept {return iIOEventPollerIdx;}

    virtual void                Close                   (void) override final;
    virtual std::string         StrEscape               (std::string_view aStr) const override final;
    virtual bool                Validate                (void) const noexcept override final;
    virtual void                SetEnv                  (const std::vector<std::tuple<std::string, std::string>>& aValues) override final;

protected:
    virtual GpDbQueryRes::SP    _Execute                (const GpDbQuery&           aQuery,
                                                         const GpDbQueryPrepared&   aQueryPrepared,
                                                         const size_t               aMinResultRowsCount) override final;

    virtual void                OnBeginTransaction      (GpDbTransactionIsolation::EnumT aIsolationLevel) override final;
    virtual void                OnCommitTransaction     (void) override final;
    virtual void                OnRollbackTransaction   (void) override final;

private:
    GpDbQueryRes::SP            ExecuteSync             (const GpDbQuery&           aQuery,
                                                         const GpDbQueryPrepared&   aQueryPrepared,
                                                         const size_t               aMinResultRowsCount);
    GpDbQueryRes::SP            ExecuteAsync            (const GpDbQuery&           aQuery,
                                                         const GpDbQueryPrepared&   aQueryPrepared,
                                                         const size_t               aMinResultRowsCount);
    void                        ClosePgConn             (void) noexcept;

private:
    PGconn*                     iPgConn         = nullptr;
    const GpIOEventPollerIdx    iIOEventPollerIdx;

    static IsolationLevelNamesT sIsolationLevelNames;
};

GpDbConnectionPgSql::GpDbConnectionPgSql
(
    PGconn*             aPgConn,
    const ModeTE        aMode,
    GpIOEventPollerIdx  aIOEventPollerIdx
) noexcept:
GpDbConnection
(
    StatusTE::CONNECTED,
    aMode
),
iPgConn          {aPgConn},
iIOEventPollerIdx{aIOEventPollerIdx}
{
}

}// namespace GPlatform

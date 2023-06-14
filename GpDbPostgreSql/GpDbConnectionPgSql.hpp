#pragma once

#include "GpDbPostgreSql_global.hpp"
#include "GpDbQueryPreparedPgSql.hpp"
#include "../../GpNetwork/GpNetworkCore/IO/Sockets/GpSocketAddr.hpp"
#include "../GpDbClient/GpDbConnection.hpp"

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

    using IsolationLevelNamesT = std::array<std::u8string_view, GpDbTransactionIsolation::SCount()>;

public:
    inline                      GpDbConnectionPgSql     (PGconn*                aPgConn,
                                                         const ModeTE           aMode,
                                                         GpIOEventPoller::SP    aEventPoller) noexcept;
    inline                      GpDbConnectionPgSql     (PGconn*            aPgConn,
                                                         const ModeTE       aMode) noexcept;
    virtual                     ~GpDbConnectionPgSql    (void) noexcept override final;

    GpSocketAddr::SocketIdT     SocketId                (void) const {return PQsocket(iPgConn);}
    PGconn*                     PgConn                  (void) {return iPgConn;}

    virtual void                Close                   (void) override final;
    virtual GpDbQueryRes::SP    Execute                 (const GpDbQuery&           aQuery,
                                                         const GpDbQueryPrepared&   aQueryPrepared,
                                                         const size_t               aMinResultRowsCount) override final;
    virtual std::u8string       StrEscape               (std::u8string_view aStr) const override final;
    virtual bool                Validate                (void) const noexcept override final;

protected:
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
    PGconn*                     iPgConn = nullptr;
    static IsolationLevelNamesT sIsolationLevelNames;
};

GpDbConnectionPgSql::GpDbConnectionPgSql
(
    PGconn*             aPgConn,
    const ModeTE        aMode,
    GpIOEventPoller::SP aEventPoller
) noexcept:
GpDbConnection
(
    StatusTE::CONNECTED,
    aMode,
    std::move(aEventPoller)
),
iPgConn(aPgConn)
{
}

GpDbConnectionPgSql::GpDbConnectionPgSql
(
    PGconn*         aPgConn,
    const ModeTE    aMode
) noexcept:
GpDbConnection(StatusTE::CONNECTED, aMode),
iPgConn(aPgConn)
{
}

}//namespace GPlatform

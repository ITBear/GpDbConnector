#pragma once

#include "GpDbPostgreSql_global.hpp"
#include "../GpDbClient/GpDbConnection.hpp"
#include "../../GpNetwork/GpNetworkCore/GpIOObjectId.hpp"
#include "../../GpNetwork/GpNetworkCore/Pollers/GpIOEventPoller.hpp"

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
    inline                      GpDbConnectionPgSql     (PGconn*            aPgConn,
                                                         const ModeTE       aMode,
                                                         GpIOEventPoller*   aIOEventPoller) noexcept;
    virtual                     ~GpDbConnectionPgSql    (void) noexcept override final;

    GpIOObjectId                SocketId                (void) const {return PQsocket(iPgConn);}
    PGconn*                     PgConn                  (void) noexcept {return iPgConn;}
    GpIOEventPoller*            IOEventPoller           (void) noexcept {return iIOEventPoller;}

    virtual void                Close                   (void) override final;
    virtual std::u8string       StrEscape               (std::u8string_view aStr) const override final;
    virtual bool                Validate                (void) const noexcept override final;
    virtual void                SetEnv                  (const std::vector<std::tuple<std::u8string, std::u8string>>& aValues) override final;

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
    GpIOEventPoller*            iIOEventPoller  = nullptr;
    static IsolationLevelNamesT sIsolationLevelNames;
};

GpDbConnectionPgSql::GpDbConnectionPgSql
(
    PGconn*             aPgConn,
    const ModeTE        aMode,
    GpIOEventPoller*    aIOEventPoller
) noexcept:
GpDbConnection
(
    StatusTE::CONNECTED,
    aMode
),
iPgConn       (aPgConn),
iIOEventPoller(aIOEventPoller)
{
}

}//namespace GPlatform

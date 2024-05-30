#pragma once

#include <GpDbConnector/GpDbPostgreSql/GpDbConnectionPgSql.hpp>
#include <GpNetwork/GpNetworkCore/Tasks/GpSingleSocketTask.hpp>
#include <postgresql/libpq-fe.h>

namespace GPlatform {

class GpDbQueryAsyncTaskPgSql final: public GpSingleSocketTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbQueryAsyncTaskPgSql)
    CLASS_DD(GpDbQueryAsyncTaskPgSql)

public:
                                GpDbQueryAsyncTaskPgSql     (GpDbConnectionPgSql&       aDbConn,
                                                             const GpDbQuery&           aQuery,
                                                             const GpDbQueryPrepared&   aQueryPrepared,
                                                             GpIOEventPollerIdx         aIOEventPollerIdx);
    virtual                     ~GpDbQueryAsyncTaskPgSql    (void) noexcept override final;

protected:
    virtual void                OnReadyToRead               (GpSocket& aSocket) override final;
    virtual void                OnReadyToWrite              (GpSocket& aSocket) override final;
    virtual void                OnClosed                    (GpSocket& aSocket) override final;
    virtual void                OnError                     (GpSocket& aSocket) override final;
    virtual void                ProcessOtherMessages        (GpAny& aMessage) override final;

private:
    GpDbConnectionPgSql&        iDbConn;
    const GpDbQuery&            iQuery;
    const GpDbQueryPrepared&    iQueryPrepared;
    bool                        iIsSend = false;
};

}// namespace GPlatform

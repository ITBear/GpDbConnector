#pragma once

#include "GpDbPostgreSql_global.hpp"
#include "GpDbConnectionPgSql.hpp"
#include <postgresql/libpq-fe.h>

namespace GPlatform {

class GpDbQueryAsyncTaskPgSql final: public GpSocketTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbQueryAsyncTaskPgSql)
    CLASS_DD(GpDbQueryAsyncTaskPgSql)

public:
                                GpDbQueryAsyncTaskPgSql     (std::string                aName,
                                                             GpDbConnectionPgSql&       aDbConn,
                                                             const GpDbQuery&           aQuery,
                                                             const GpDbQueryPrepared&   aQueryPrepared,
                                                             const size_t               aMinResultRowsCount);
    virtual                     ~GpDbQueryAsyncTaskPgSql    (void) noexcept override final;

    virtual GpTaskDoRes         OnSockReadyToRead           (GpSocket& aSocket) override final;
    virtual GpTaskDoRes         OnSockReadyToWrite          (GpSocket& aSocket) override final;
    virtual void                OnSockClosed                (GpSocket& aSocket) override final;
    virtual void                OnSockError                 (GpSocket& aSocket) override final;

private:
    GpTaskDoRes                 ProcessR                    (void);
    GpTaskDoRes                 ProcessW                    (void);
    void                        Send                        (void);

private:
    GpDbConnectionPgSql&        iDbConn;
     const GpDbQuery&           iQuery;
    const GpDbQueryPrepared&    iQueryPrepared;
    const size_t                iMinResultRowsCount;
    bool                        iIsSend = false;
};

}//namespace GPlatform

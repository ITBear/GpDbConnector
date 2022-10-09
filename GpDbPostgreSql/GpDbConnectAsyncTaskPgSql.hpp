#pragma once

#include "GpDbPostgreSql_global.hpp"
#include <postgresql/libpq-fe.h>

namespace GPlatform {

class GpDbConnectAsyncTaskPgSql final: public GpSocketTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDbConnectAsyncTaskPgSql)
    CLASS_DD(GpDbConnectAsyncTaskPgSql)

public:
                            GpDbConnectAsyncTaskPgSql   (std::string            aName,
                                                         GpIOEventPoller::SP    aIOPoller,
                                                         PGconn*                aPGconn);
    virtual                 ~GpDbConnectAsyncTaskPgSql  (void) noexcept override final;

    virtual GpTaskDoRes     OnSockReadyToRead           (GpSocket& aSocket) override final;
    virtual GpTaskDoRes     OnSockReadyToWrite          (GpSocket& aSocket) override final;
    virtual void            OnSockClosed                (GpSocket& aSocket) override final;
    virtual void            OnSockError                 (GpSocket& aSocket) override final;

private:
    GpTaskDoRes             ProcessRW                   (void);

private:
    PGconn*                 iPGconn = nullptr;
};

}//namespace GPlatform

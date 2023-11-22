TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 2
VER_MIN		    = 1
VER_PAT		    = 4
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=c192f513-2e74-42f6-b3f9-c86c92d72624
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
PACKET_NAME     = GpDbClient
DIR_LEVEL       = ./../..

DEFINES		   += GP_DB_CLIENT_LIBRARY
DEFINES        += "GP_CURRENT_LIB_VER_MAJ=\\\"$$VER_MAJ\\\""
DEFINES        += "GP_CURRENT_LIB_VER_MIN=\\\"$$VER_MIN\\\""
DEFINES        += "GP_CURRENT_LIB_VER_PAT=\\\"$$VER_PAT\\\""
DEFINES        += "GP_CURRENT_LIB_PACKET_NAME=\\\"$$PACKET_NAME\\\""

include(../../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
}

os_linux{
}
#------------------------------- LIBS END ----------------------------------

HEADERS += \
	GpDbClient.hpp \
	GpDbClient_global.hpp \
	GpDbConnection.hpp \
	GpDbConnectionGuard.hpp \
	GpDbConnectionHookManager.hpp \
	GpDbConnectionMode.hpp \
	GpDbConnectionStatus.hpp \
	GpDbDriver.hpp \
	GpDbDriverCatalog.hpp \
	GpDbDriverFactory.hpp \
	GpDbException.hpp \
	GpDbExceptionCode.hpp \
	GpDbManager.hpp \
	GpDbManagerCatalog.hpp \
	GpDbManagerCfgDesc.hpp \
	GpDbTransactionIsolation.hpp \
	Query/GpDbQuery.hpp \
	Query/GpDbQueryBuilder.hpp \
	Query/GpDbQueryMapperCache.hpp \
	Query/GpDbQueryPrepared.hpp \
	Query/GpDbQueryRes.hpp \
	Query/GpDbQueryResState.hpp \
	Query/GpDbQuerySearchBuilder.hpp \
	Query/GpDbQueryValType.hpp \
	Query/GpDbQueryValue.hpp

SOURCES += \
	GpDbClient.cpp \
	GpDbConnection.cpp \
	GpDbConnectionGuard.cpp \
	GpDbConnectionHookManager.cpp \
	GpDbConnectionMode.cpp \
	GpDbConnectionStatus.cpp \
	GpDbDriverCatalog.cpp \
	GpDbException.cpp \
	GpDbExceptionCode.cpp \
	GpDbManager.cpp \
	GpDbManagerCatalog.cpp \
	GpDbManagerCfgDesc.cpp \
	GpDbTransactionIsolation.cpp \
	Query/GpDbQuery.cpp \
	Query/GpDbQueryBuilder.cpp \
	Query/GpDbQueryMapperCache.cpp \
	Query/GpDbQueryRes.cpp \
	Query/GpDbQueryResState.cpp \
	Query/GpDbQuerySearchBuilder.cpp \
	Query/GpDbQueryValType.cpp

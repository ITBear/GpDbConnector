# ----------- Config -----------
TEMPLATE        = lib
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=c192f513-2e74-42f6-b3f9-c86c92d72624
PACKET_NAME     = GpDbClient
DEFINES        += GP_DB_CLIENT_LIBRARY
_VER_MAJ        = 2
_VER_MIN        = 1
_VER_PAT        = 6
DIR_LEVEL       = ./../..

include($$DIR_LEVEL/../QtGlobalPro.pri)

equals(var_link, "static") {
	CONFIG += staticlib
}

# ----------- Libraries -----------
equals(var_os, "windows") {
	LIBS += -lGpDbDataModels$$TARGET_POSTFIX
	LIBS += -lGpLogCore$$TARGET_POSTFIX
	LIBS += -lGpNetworkCore$$TARGET_POSTFIX
	LIBS += -lGpJson$$TARGET_POSTFIX
	LIBS += -lGpReflection$$TARGET_POSTFIX
	LIBS += -lGpTasks$$TARGET_POSTFIX
	LIBS += -lGpUtils$$TARGET_POSTFIX
}

equals(var_os, "linux") {
	LIBS += -lGpDbDataModels$$TARGET_POSTFIX
	LIBS += -lGpLogCore$$TARGET_POSTFIX
	LIBS += -lGpNetworkCore$$TARGET_POSTFIX
	LIBS += -lGpJson$$TARGET_POSTFIX
	LIBS += -lGpReflection$$TARGET_POSTFIX
	LIBS += -lGpTasks$$TARGET_POSTFIX
	LIBS += -lGpUtils$$TARGET_POSTFIX

	LIBS += -lfmt
}

# ----------- Sources and headers -----------
HEADERS += \
	GpDbClientLib.hpp \
	GpDbClient_global.hpp \
	GpDbConnection.hpp \
	GpDbConnectionGuard.hpp \
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
	Query/GpDbQuerySearchBuilder.hpp \
	Query/GpDbQueryValType.hpp \
	Query/GpDbQueryValue.hpp

SOURCES += \
	GpDbClientLib.cpp \
	GpDbConnection.cpp \
	GpDbConnectionGuard.cpp \
	GpDbDriver.cpp \
	GpDbDriverCatalog.cpp \
	GpDbDriverFactory.cpp \
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
	Query/GpDbQuerySearchBuilder.cpp \
	Query/GpDbQueryValType.cpp

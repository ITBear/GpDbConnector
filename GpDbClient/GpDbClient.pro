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

release_build_static{
	CONFIG += staticlib
}

# ----------- Libraries -----------
os_windows{
	LIBS += -lGpDbDataModels$$TARGET_POSTFIX
	LIBS += -lGpLogCore$$TARGET_POSTFIX
	LIBS += -lGpNetworkCore$$TARGET_POSTFIX
	LIBS += -lGpJson$$TARGET_POSTFIX
	LIBS += -lGpReflection$$TARGET_POSTFIX
	LIBS += -lGpTasks$$TARGET_POSTFIX
	LIBS += -lGpUtils$$TARGET_POSTFIX

	QMAKE_LFLAGS += /NODEFAULTLIB:libboost_fiber-vc143-mt-x64-1_84
	QMAKE_LFLAGS += /NODEFAULTLIB:libboost_fiber-vc143-mt-gd-x64-1_84
	QMAKE_LFLAGS += /NODEFAULTLIB:boost_fiber-vc143-mt-gd-x64-1_84
	LIBS +=  -lboost_fiber-vc143-mt-x64-1_84
	QMAKE_LFLAGS += /NODEFAULTLIB:libboost_context-vc143-mt-x64-1_84
	QMAKE_LFLAGS += /NODEFAULTLIB:libboost_context-vc143-mt-gd-x64-1_84
	QMAKE_LFLAGS += /NODEFAULTLIB:boost_context-vc143-mt-gd-x64-1_84
	LIBS += -lboost_context-vc143-mt-x64-1_84
}

os_linux{
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
	Query/GpDbQueryResState.hpp \
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
	Query/GpDbQueryResState.cpp \
	Query/GpDbQuerySearchBuilder.cpp \
	Query/GpDbQueryValType.cpp

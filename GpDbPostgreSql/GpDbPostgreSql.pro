# ----------- Config -----------
TEMPLATE        = lib
#CONFIG        += staticlib
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=ba443106-9992-4c29-93db-6e29aef8c576
PACKET_NAME     = GpDbPostgreSql
DEFINES        += GP_DB_POSTGRESQL_LIBRARY
_VER_MAJ        = 2
_VER_MIN        = 1
_VER_PAT        = 5
DIR_LEVEL       = ./../..

include($$DIR_LEVEL/../QtGlobalPro.pri)

# ----------- Libraries -----------
os_windows{
}

os_linux{
}

# ----------- Sources and headers -----------
SOURCES += \
	GpDbConnectionPgSql.cpp \
	GpDbDriverFactoryPgSql.cpp \
	GpDbDriverPgSql.cpp \
	GpDbPostgreSqlLib.cpp \
	GpDbQueryAsyncTaskPgSql.cpp \
	GpDbQueryPreparedPgSql.cpp \
	GpDbQueryResPgSql.cpp

HEADERS += \
	GpDbArrayUtilsPgSql.hpp \
	GpDbConnectionPgSql.hpp \
	GpDbDriverFactoryPgSql.hpp \
	GpDbDriverPgSql.hpp \
	GpDbPostgreSqlLib.hpp \
	GpDbPostgreSql_global.hpp \
	GpDbQueryAsyncTaskPgSql.hpp \
	GpDbQueryPreparedPgSql.hpp \
	GpDbQueryResPgSql.hpp

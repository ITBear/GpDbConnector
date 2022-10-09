TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 0
VER_MIN		    = 1
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=ba443106-9992-4c29-93db-6e29aef8c576
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
DEFINES		   += GP_DB_POSTGRESQL_LIBRARY
PACKET_NAME     = GpDbPostgreSql
DIR_LEVEL       = ./../..

include(../../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
}

os_linux{
}

LIBS += -lpq
#------------------------------- LIBS END ----------------------------------

SOURCES += \
	GpDbArrayUtilsPgSql.cpp \
	GpDbConnectAsyncTaskPgSql.cpp \
	GpDbConnectionPgSql.cpp \
	GpDbDriverFactoryPgSql.cpp \
	GpDbDriverPgSql.cpp \
	GpDbQueryAsyncTaskPgSql.cpp \
	GpDbQueryPreparedPgSql.cpp \
	GpDbQueryResPgSql.cpp

HEADERS += \
	GpDbArrayUtilsPgSql.hpp \
	GpDbConnectAsyncTaskPgSql.hpp \
	GpDbConnectionPgSql.hpp \
	GpDbDriverFactoryPgSql.hpp \
	GpDbDriverPgSql.hpp \
	GpDbPostgreSql.hpp \
	GpDbPostgreSql_global.hpp \
	GpDbQueryAsyncTaskPgSql.hpp \
	GpDbQueryPreparedPgSql.hpp \
	GpDbQueryResPgSql.hpp

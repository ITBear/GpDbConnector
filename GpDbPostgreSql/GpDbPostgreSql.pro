TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 2
VER_MIN		    = 1
VER_PAT		    = 4
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=ba443106-9992-4c29-93db-6e29aef8c576
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
PACKET_NAME     = GpDbPostgreSql
DIR_LEVEL       = ./../..

DEFINES		   += GP_DB_POSTGRESQL_LIBRARY
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

LIBS += -lpq
#------------------------------- LIBS END ----------------------------------

SOURCES += \
	GpDbConnectionPgSql.cpp \
	GpDbDriverFactoryPgSql.cpp \
	GpDbDriverPgSql.cpp \
	GpDbPostgreSql.cpp \
	GpDbQueryAsyncTaskPgSql.cpp \
	GpDbQueryPreparedPgSql.cpp \
	GpDbQueryResPgSql.cpp

HEADERS += \
	GpDbArrayUtilsPgSql.hpp \
	GpDbConnectionPgSql.hpp \
	GpDbDriverFactoryPgSql.hpp \
	GpDbDriverPgSql.hpp \
	GpDbPostgreSql.hpp \
	GpDbPostgreSql_global.hpp \
	GpDbQueryAsyncTaskPgSql.hpp \
	GpDbQueryPreparedPgSql.hpp \
	GpDbQueryResPgSql.hpp

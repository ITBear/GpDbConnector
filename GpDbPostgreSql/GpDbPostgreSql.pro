# ----------- Config -----------
TEMPLATE        = lib
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=ba443106-9992-4c29-93db-6e29aef8c576
PACKET_NAME     = GpDbPostgreSql
DEFINES        += GP_DB_POSTGRESQL_LIBRARY
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
}

os_linux{
}

# ----------- Sources and headers -----------
SOURCES += \
	GpDbConnectionPgSql.cpp \
	GpDbConnectionTaskPgSql.cpp \
	GpDbDriverFactoryPgSql.cpp \
	GpDbDriverPgSql.cpp \
	GpDbPostgreSqlLib.cpp \
	GpDbQueryPreparedPgSql.cpp \
	GpDbQueryResPgSql.cpp \
	PsqlProtocol/GpPsqlErrorResponseUtils.cpp \
	PsqlProtocol/GpPsqlMessageProcessor.cpp \
	PsqlProtocol/GpPsqlProtocolDeserializer.cpp \
	PsqlProtocol/GpPsqlProtocolMessages.cpp \
	PsqlProtocol/GpPsqlProtocolSerializer.cpp \
	PsqlProtocol/GpPsqlScramSha256.cpp

HEADERS += \
	GpDbArrayUtilsPgSql.hpp \
	GpDbConnectionPgSql.hpp \
	GpDbConnectionTaskPgSql.hpp \
	GpDbDriverFactoryPgSql.hpp \
	GpDbDriverPgSql.hpp \
	GpDbPostgreSqlLib.hpp \
	GpDbPostgreSql_global.hpp \
	GpDbQueryPreparedPgSql.hpp \
	GpDbQueryResPgSql.hpp \
	PsqlProtocol/GpPsqlErrorResponseUtils.hpp \
	PsqlProtocol/GpPsqlMessageProcessor.hpp \
	PsqlProtocol/GpPsqlProtocolDeserializer.hpp \
	PsqlProtocol/GpPsqlProtocolMessages.hpp \
	PsqlProtocol/GpPsqlProtocolSerializer.hpp \
	PsqlProtocol/GpPsqlScramSha256.hpp

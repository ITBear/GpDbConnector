# ----------- Config -----------
TEMPLATE        = lib
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=cc435efe-7b27-40e0-bc92-3f3c0c2cfe3b
PACKET_NAME     = GpDbDataModels
DEFINES        += GP_DB_DATA_MODELS_LIBRARY
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
	LIBS += -lGpReflection$$TARGET_POSTFIX
	LIBS += -lGpUtils$$TARGET_POSTFIX
}

os_linux{
}

# ----------- Sources and headers -----------
HEADERS += \
	GpDbDataModelsLib.hpp \
	GpDbDataModels_global.hpp \
	Search/GpDbDataModelsSearch.hpp \
	Search/GpDbSearchDesc.hpp \
	Search/GpDbSearchOrderDesc.hpp \
	Search/GpDbSearchOrderType.hpp

SOURCES += \
	GpDbDataModelsLib.cpp \
	Search/GpDbSearchDesc.cpp \
	Search/GpDbSearchOrderDesc.cpp \
	Search/GpDbSearchOrderType.cpp

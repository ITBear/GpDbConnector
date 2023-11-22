TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 2
VER_MIN		    = 1
VER_PAT		    = 4
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=cc435efe-7b27-40e0-bc92-3f3c0c2cfe3b
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
PACKET_NAME     = GpDbDataModels
DIR_LEVEL       = ./../..

DEFINES		   += GP_DB_DATA_MODELS_LIBRARY
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
	GpDbDataModels.hpp \
	GpDbDataModels_global.hpp \
	Search/GpDbDataModelsSearch.hpp \
	Search/GpDbSearchDesc.hpp \
	Search/GpDbSearchOrderDesc.hpp \
	Search/GpDbSearchOrderType.hpp

SOURCES += \
	GpDbDataModels.cpp \
	Search/GpDbSearchDesc.cpp \
	Search/GpDbSearchOrderDesc.cpp \
	Search/GpDbSearchOrderType.cpp

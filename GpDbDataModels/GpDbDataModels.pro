TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 0
VER_MIN		    = 1
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=cc435efe-7b27-40e0-bc92-3f3c0c2cfe3b
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
DEFINES		   += GP_DB_DATA_MODELS_LIBRARY
PACKET_NAME     = GpDbDataModels
DIR_LEVEL       = ./../..

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
	Search/GpDbSearchDesc.cpp \
	Search/GpDbSearchOrderDesc.cpp \
	Search/GpDbSearchOrderType.cpp

#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroImportExport.hpp>

#if defined(GP_DB_DATA_MODELS_LIBRARY)
    #define GP_DB_DATA_MODELS_API GP_DECL_EXPORT
#else
    #define GP_DB_DATA_MODELS_API GP_DECL_IMPORT
#endif

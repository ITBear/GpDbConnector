#pragma once

#include "../../GpCore2/GpUtils/Macro/GpMacroImportExport.hpp"

#if defined(GP_DB_CLIENT_LIBRARY)
    #define GP_DB_CLIENT_API GP_DECL_EXPORT
#else
    #define GP_DB_CLIENT_API GP_DECL_IMPORT
#endif

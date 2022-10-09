#pragma once

#include "../../GpCore2/GpUtils/GpUtils.hpp"
#include "../../GpCore2/GpTasks/GpTasks.hpp"
#include "../../GpNetwork/GpNetworkCore/GpNetworkCore.hpp"
#include "../GpDbDataModels/GpDbDataModels.hpp"

#if defined(GP_DB_CLIENT_LIBRARY)
    #define GP_DB_CLIENT_API GP_DECL_EXPORT
#else
    #define GP_DB_CLIENT_API GP_DECL_IMPORT
#endif

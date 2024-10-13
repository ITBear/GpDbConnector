TEMPLATE = subdirs

os_linux {
	SUBDIRS += \
		GpDbDataModels \
		GpDbClient \
		GpDbPostgreSql
} else:os_android {
	SUBDIRS += \
		GpDbDataModels \
		GpDbClient \
		GpDbPostgreSql
} else:os_ios {
	SUBDIRS += \
		GpDbDataModels \
		GpDbClient \
		GpDbPostgreSql
} else:os_windows {
	SUBDIRS += \
		GpDbDataModels \
		GpDbClient
#		GpDbPostgreSql
} else:os_macx {
	SUBDIRS += \
		GpDbDataModels \
		GpDbClient \
		GpDbPostgreSql
} else:os_browser {
	SUBDIRS +=
} else {
    error("Unknown OS")
}

CONFIG += ordered

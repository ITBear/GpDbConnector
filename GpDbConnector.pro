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
	error("Unknown OS. Set CONFIG+=... one of values: os_linux, os_android, os_ios, os_windows, os_macx, os_browser, os_baremetal")
}

CONFIG += ordered

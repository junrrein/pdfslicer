if (MINGW)
	set (ICON_DIR_RELATIVE "share/icons/Adwaita")
else ()
	set (ICON_DIR_RELATIVE "share/icons/hicolor")
endif ()

install (FILES pdfslicer-symbolic.svg
		DESTINATION ${ICON_DIR_RELATIVE}/scalable/apps)


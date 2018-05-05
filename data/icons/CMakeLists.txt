file (COPY pdfslicer.ico
	DESTINATION ${CMAKE_BINARY_DIR})

if (MINGW)
	set (ICON_DIR_RELATIVE "share/icons/Adwaita")
else ()
	set (ICON_DIR_RELATIVE "share/icons/hicolor")
endif ()

install (FILES ${APPLICATION_ID}.svg
		DESTINATION ${ICON_DIR_RELATIVE}/scalable/apps)

if (MINGW)
	set (ICON_DIR_RELATIVE "share/icons/Adwaita")

	file (COPY pdfslicer.ico DESTINATION ${CMAKE_BINARY_DIR})
	file (COPY pdfslicer.ico DESTINATION ${CMAKE_BINARY_DIR}/src)
else ()
	set (ICON_DIR_RELATIVE "share/icons/hicolor")
endif ()

install (FILES
		${APPLICATION_ID}.svg
		DESTINATION ${ICON_DIR_RELATIVE}/scalable/apps)

install (FILES
		DESTINATION ${ICON_DIR_RELATIVE}/scalable/actions)

configure_file (pdfslicer.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/pdfslicer.desktop)

if (NOT MINGW)
	install (FILES ${CMAKE_CURRENT_BINARY_DIR}/pdfslicer.desktop
		DESTINATION share/applications)
endif ()

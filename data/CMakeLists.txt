include (Translations)

configure_file_translation (pdfslicer.desktop.in
	${CMAKE_CURRENT_BINARY_DIR}/pdfslicer.desktop
	${CMAKE_SOURCE_DIR}/po
)

if (NOT MINGW)
	install (FILES ${CMAKE_CURRENT_BINARY_DIR}/pdfslicer.desktop
		DESTINATION share/applications)
endif ()

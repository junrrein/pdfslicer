include (Translations)

configure_file (${APPLICATION_ID}.desktop.in
				${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_ID}.desktop.in)

configure_file_translation (${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_ID}.desktop.in
							${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_ID}.desktop
                            ${CMAKE_SOURCE_DIR}/po)

if (NOT MINGW)
	install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_ID}.desktop
                   DESTINATION
                   share/applications)
endif ()

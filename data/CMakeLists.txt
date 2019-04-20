add_subdirectory (icons)

include (Translations)

configure_file (${APPLICATION_ID}.desktop.in
				${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_ID}.desktop.in)
configure_file (${APPLICATION_ID}.appdata.xml.in
				${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_ID}.appdata.xml.in)

configure_file_translation (${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_ID}.desktop.in
							${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_ID}.desktop
							${CMAKE_SOURCE_DIR}/po)
configure_file_translation (${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_ID}.appdata.xml.in
							${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_ID}.appdata.xml
							${CMAKE_SOURCE_DIR}/po)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_ID}.desktop
			   DESTINATION
			   share/applications)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_ID}.appdata.xml
			  DESTINATION
			  share/metainfo)

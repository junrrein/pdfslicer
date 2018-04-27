include (Translations)

configure_file (pdfslicer.desktop.in
                ${CMAKE_CURRENT_BINARY_DIR}/pdfslicer.desktop.in)

configure_file_translation (${CMAKE_CURRENT_BINARY_DIR}/pdfslicer.desktop.in
                            ${CMAKE_CURRENT_BINARY_DIR}/pdfslicer.desktop
                            ${CMAKE_SOURCE_DIR}/po)

if (NOT MINGW)
    install (FILES ${CMAKE_CURRENT_BINARY_DIR}/pdfslicer.desktop
                   DESTINATION
                   share/applications)
endif ()

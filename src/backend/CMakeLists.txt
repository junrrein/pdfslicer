configure_file (config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp)

set (SOURCES
	 ${CMAKE_CURRENT_SOURCE_DIR}/command.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/commandmanager.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/config.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/document.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/page.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/pdfannotationswriter.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/pdfsaver.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/pagerenderer.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/tempfile.cpp)

add_library (backend STATIC ${SOURCES})

target_include_directories (backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (backend SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/third-party/PDF-Writer/PDFWriter)

target_link_libraries_system (backend stduuid)
target_link_libraries (backend PDFWriter)

if (MINGW)
	target_include_directories (backend PUBLIC
		${GTKMM_INCLUDE_DIRS}
		${POPPLER_INCLUDE_DIRS})
	target_link_libraries (backend
		${GTKMM_LDFLAGS}
		${POPPLER_LDFLAGS})
else ()
	target_link_libraries_system (backend
		PkgConfig::GTKMM
		PkgConfig::POPPLER
		PkgConfig::QPDF)
endif ()

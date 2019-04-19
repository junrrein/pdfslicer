configure_file (config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp)

set (SOURCES
	 ${CMAKE_CURRENT_SOURCE_DIR}/command.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/commandmanager.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/config.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/document.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/page.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/pdfsaver.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/pagerenderer.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/tempfile.cpp)

add_library (backend STATIC ${SOURCES})

target_include_directories (backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries_system (backend
	stduuid
	PkgConfig::QPDF)

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
		PkgConfig::POPPLER)
endif ()

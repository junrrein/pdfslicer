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

if (MINGW)
#	target_include_directories (backend PUBLIC
#		${GTKMM_INCLUDE_DIRS}
#		${POPPLER_INCLUDE_DIRS})
#	target_link_libraries (backend
#		${GTKMM_LDFLAGS}
#		${POPPLER_LDFLAGS})
	target_link_libraries (backend
		stduuid
		range-v3
		PkgConfig::GTKMM
		PkgConfig::POPPLER
		PkgConfig::QPDF)
else ()
	target_link_libraries_system (backend
		stduuid
		range-v3
		PkgConfig::GTKMM
		PkgConfig::POPPLER
		PkgConfig::QPDF)
endif ()

target_compile_options(backend PUBLIC $<$<CONFIG:DEBUG>:${SLICER_DEBUG_FLAGS}>)

configure_file (config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp)

set (SOURCES
	 ${CMAKE_CURRENT_SOURCE_DIR}/command.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/commandmanager.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/config.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/document.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/fileutils.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/page.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/pdfsaver.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/pagerenderer.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/tempfile.cpp)

add_library (backend STATIC ${SOURCES})

target_include_directories (backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries_system (backend
	stduuid
	range-v3
	PkgConfig::GTKMM
	PkgConfig::POPPLER
	PkgConfig::QPDF)

target_compile_options(backend PUBLIC $<$<CONFIG:DEBUG>:${SLICER_DEBUG_FLAGS}>)

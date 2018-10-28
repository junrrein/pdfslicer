add_library (stduuid INTERFACE)
target_include_directories (stduuid INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (stduuid INTERFACE GSL)

# FIXME: In Apple platforms, we need to link against CoreFoundation
if (NOT WIN32)
	pkg_check_modules (ossp-uuid uuid IMPORTED_TARGET REQUIRED)
	target_link_libraries (stduuid INTERFACE PkgConfig::ossp-uuid)
endif ()

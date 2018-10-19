add_library (stduuid INTERFACE)
target_sources (stduuid INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/uuid.h)
target_link_libraries (stduuid INTERFACE GSL)

# FIXME: In Apple platforms, we need to link against CoreFoundation
if (NOT WIN32)
	pkg_check_modules (ossp-uuid uuid REQUIRED)
	target_link_libraries (stduuid INTERFACE uuid)
endif ()

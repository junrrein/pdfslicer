add_library (stduuid INTERFACE)
target_sources (stduuid INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/uuid.h)
target_link_libraries (stduuid INTERFACE GSL)

if (NOT WIN32)
	target_link_libraries (stduuid INTERFACE uuid)
endif ()

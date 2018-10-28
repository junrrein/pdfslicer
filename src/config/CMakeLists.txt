configure_file (config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp)

set (SOURCES
	config.hpp
	config.cpp)

add_library (config STATIC ${SOURCES})

target_include_directories (config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (MINGW)
	target_include_directories (config PUBLIC ${GTKMM_INCLUDE_DIRS})
else ()
	target_include_directories (config SYSTEM PUBLIC ${GTKMM_INCLUDE_DIRS})
endif ()

target_link_libraries (config ${GTKMM_LDFLAGS})

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	   set_target_properties (config PROPERTIES COMPILE_FLAGS ${GNU_COMPILE_FLAGS})
endif ()

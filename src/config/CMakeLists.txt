configure_file (config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp)

set (SOURCES
	config.hpp
	config.cpp)

add_library (config STATIC ${SOURCES})

target_include_directories (config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (config PkgConfig::GTKMM)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	   set_target_properties (config PROPERTIES COMPILE_FLAGS ${GNU_COMPILE_FLAGS})
endif ()

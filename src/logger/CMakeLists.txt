set (SOURCES
	logger.hpp
	logger.cpp)

add_library (logger STATIC ${SOURCES})

target_include_directories (logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries_system (logger
	config
	spdlog)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	   set_target_properties (logger PROPERTIES COMPILE_FLAGS ${GNU_COMPILE_FLAGS})
endif ()

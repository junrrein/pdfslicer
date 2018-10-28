set (SOURCES
	logger.hpp
	logger.cpp)

add_library (logger STATIC ${SOURCES})

target_include_directories (logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories (
	logger SYSTEM PUBLIC
	config
	${CMAKE_SOURCE_DIR}/third-party/spdlog/include
)

if (MINGW)
	target_include_directories (logger PRIVATE ${GTKMM_INCLUDE_DIRS})
else ()
	target_include_directories (logger SYSTEM PRIVATE ${GTKMM_INCLUDE_DIRS})
endif ()

target_link_libraries (logger
	config
	${GTKMM_LDFLAGS})

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	   set_target_properties (logger PROPERTIES COMPILE_FLAGS ${GNU_COMPILE_FLAGS})
endif ()

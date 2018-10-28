set (SOURCES
	main.cpp)

add_executable (tests ${SOURCES})

target_link_libraries (tests
	Catch2)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	   set_target_properties (tests PROPERTIES COMPILE_FLAGS ${GNU_COMPILE_FLAGS})
endif ()

set (SOURCES
	main.cpp
	backend.tempfile.cpp)
get_target_property (SLICER_INCLUDE_DIRS ${CMAKE_PROJECT_NAME} INTERFACE_INCLUDE_DIRECTORIES)

add_executable (tests ${SOURCES})
target_include_directories (tests PRIVATE ${SLICER_INCLUDE_DIRS})
target_link_libraries_system (tests
	${CMAKE_PROJECT_NAME}
	Catch2)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	   set_target_properties (tests PROPERTIES COMPILE_FLAGS ${GNU_COMPILE_FLAGS})
endif ()

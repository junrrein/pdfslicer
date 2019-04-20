set (SOURCES
	main.cpp
	command.cpp
	document.cpp
	tempfile.cpp)

add_executable (pdfslicer_tests ${SOURCES})
target_link_libraries_system (pdfslicer_tests
	backend
	Catch2)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	   set_target_properties (pdfslicer_tests PROPERTIES COMPILE_FLAGS ${GNU_COMPILE_FLAGS})
endif ()

file (GLOB SLICER_TEST_MATERIALS "materials/*.pdf")
file (COPY ${SLICER_TEST_MATERIALS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

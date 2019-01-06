set (SOURCES
	main.cpp
	backend.tempfile.cpp)

add_executable (pdfslicer_tests ${SOURCES})
target_link_libraries_system (pdfslicer_tests Catch2)

if (MINGW)
	target_link_libraries (pdfslicer_tests backend)
else ()
	target_link_libraries_system (pdfslicer_tests backend)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	   set_target_properties (pdfslicer_tests PROPERTIES COMPILE_FLAGS ${GNU_COMPILE_FLAGS})
endif ()

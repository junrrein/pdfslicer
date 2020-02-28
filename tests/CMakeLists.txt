set (SOURCES
	main.cpp
	command.cpp
	document.addfile.cpp
	document.move.cpp
	document.remove.cpp
	tempfile.cpp)

add_executable (pdfslicer_tests ${SOURCES})
target_link_libraries_system (pdfslicer_tests
	backend
	Catch2)

target_compile_options(pdfslicer_tests PUBLIC $<$<CONFIG:DEBUG>:${SLICER_DEBUG_FLAGS}>)

file (GLOB SLICER_TEST_MATERIALS "materials/*.pdf")
file (COPY ${SLICER_TEST_MATERIALS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

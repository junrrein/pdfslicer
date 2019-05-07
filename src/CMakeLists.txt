include (GResource)
gresource (${SOURCE_DATA_DIR} pdfslicer.gresource.xml
	 ${CMAKE_CURRENT_BINARY_DIR} GLIB_RESOURCE_FILE)

add_executable (${CMAKE_PROJECT_NAME}
	main.cpp
	${GLIB_RESOURCE_FILE})

add_subdirectory (application)
add_subdirectory (backend)
add_subdirectory (logger)

add_compile_definitions ("GETTEXT_PACKAGE=${GETTEXT_PACKAGE}")
add_compile_definitions (SPDLOG_FMT_EXTERNAL)

target_link_libraries_system (${CMAKE_PROJECT_NAME}
	backend
	fmt::fmt-header-only
	Safe
	spdlog
	ThreadPool)

target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC $<$<CONFIG:DEBUG>:${SLICER_DEBUG_FLAGS}>)

install (TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)

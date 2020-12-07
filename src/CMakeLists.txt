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

if (MINGW)
	target_link_libraries (${CMAKE_PROJECT_NAME}
		backend
		fmt::fmt-header-only
		Safe
		spdlog
		ThreadPool)
else ()
	target_link_libraries_system (${CMAKE_PROJECT_NAME}
		backend
		fmt::fmt-header-only
		Safe
		spdlog
		ThreadPool)
endif ()

if (MINGW)
	target_sources (${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/pdfslicer.rc)

	if (CMAKE_BUILD_TYPE STREQUAL "Release")
		target_link_libraries (${PROJECT_NAME} "-mwindows")
	endif ()
endif ()

target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC $<$<CONFIG:DEBUG>:${SLICER_DEBUG_FLAGS}>)

install (TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)

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
	fmt
	range-v3
	spdlog
	ThreadPool)

if (MINGW)
	target_link_libraries (${CMAKE_PROJECT_NAME} backend)
else ()
	target_link_libraries_system (${CMAKE_PROJECT_NAME} backend)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES COMPILE_FLAGS ${GNU_COMPILE_FLAGS})
    endif ()

#	 if (CLANG_TIDY_EXE)
#		 set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
#	 endif ()
endif ()

if (MINGW)
	target_sources (${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/pdfslicer.rc)

	if (CMAKE_BUILD_TYPE STREQUAL "Release")
		target_link_libraries (${PROJECT_NAME} "-mwindows")
	endif ()
endif ()

install (TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)

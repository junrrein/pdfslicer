add_executable (${CMAKE_PROJECT_NAME}
	main.cpp)
set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES ENABLE_EXPORTS ON)

add_subdirectory (application)
add_subdirectory (config)
add_subdirectory (backend)
add_subdirectory (logger)

include (GResource)
gresource (${SOURCE_DATA_DIR} pdfslicer.gresource.xml
	 ${CMAKE_CURRENT_BINARY_DIR} GLIB_RESOURCE_FILE)
target_sources (${CMAKE_PROJECT_NAME} PUBLIC ${GLIB_RESOURCE_FILE})

add_compile_definitions ("GETTEXT_PACKAGE=${GETTEXT_PACKAGE}")

target_include_directories (
	${CMAKE_PROJECT_NAME} SYSTEM PUBLIC
	${CMAKE_SOURCE_DIR}/third-party/PDF-Writer/PDFWriter
)

target_link_libraries_system (${CMAKE_PROJECT_NAME}
	range-v3
	spdlog
	stduuid
	ThreadPool
	PkgConfig::GTKMM
	PkgConfig::POPPLER)
target_link_libraries (${CMAKE_PROJECT_NAME} PDFWriter)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES COMPILE_FLAGS ${GNU_COMPILE_FLAGS})
    endif ()

#	 if (CLANG_TIDY_EXE)
#		 set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
#	 endif ()
endif ()

if (MINGW)
	target_sources (${CMAKE_PROJECT_NAME} PUBLIC pdfslicer.rc)

	if (CMAKE_BUILD_TYPE STREQUAL "Release")
		target_link_libraries (${PROJECT_NAME} "-mwindows")
	endif ()
endif ()

install (TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)

add_subdirectory (config)
add_subdirectory (backend)
add_subdirectory (logger)

set (APPLICATION_SOURCES
	application/application.hpp
	application/application.cpp
	application/backgroundthread.hpp
	application/backgroundthread.cpp
	application/settingsmanager.hpp
	application/settingsmanager.cpp
	application/aboutdialog.hpp
	application/aboutdialog.cpp
	application/actionbar.hpp
	application/actionbar.cpp
	application/appwindow.hpp
	application/appwindow.cpp
	application/filechoosernative.hpp
	application/filechoosernative.cpp
	application/headerbar.hpp
	application/headerbar.cpp
	application/interactivepagewidget.hpp
	application/interactivepagewidget.cpp
	application/openfiledialog.hpp
	application/openfiledialog.cpp
	application/pagewidget.hpp
	application/pagewidget.cpp
	application/previewwindow.hpp
	application/previewwindow.cpp
	application/savefiledialog.hpp
	application/savefiledialog.cpp
	application/savingrevealer.hpp
	application/savingrevealer.cpp
	application/utils.hpp
	application/utils.cpp
	application/view.hpp
	application/view.cpp
	application/welcomescreen.hpp
	application/welcomescreen.cpp
	application/zoomlevel.hpp
	application/zoomlevel.cpp
	application/zoomlevelwithactions.hpp
	application/zoomlevelwithactions.cpp)

include (GResource)
gresource (${SOURCE_DATA_DIR} pdfslicer.gresource.xml
	 ${CMAKE_CURRENT_BINARY_DIR} GLIB_RESOURCE_FILE)

add_executable (${CMAKE_PROJECT_NAME}
				main.cpp
				${APPLICATION_SOURCES}
				${GLIB_RESOURCE_FILE})
add_definitions ("-DGETTEXT_PACKAGE=\"${GETTEXT_PACKAGE}\"")

target_link_libraries_system (${CMAKE_PROJECT_NAME}
	config
	backend
	logger
	ThreadPool
	range-v3
	PkgConfig::GTKMM
	PkgConfig::POPPLER)

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

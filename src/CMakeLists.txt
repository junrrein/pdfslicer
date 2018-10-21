add_subdirectory (backend)
add_subdirectory (logger)

set (SLICER_APPLICATION_SOURCES
	application/application.hpp
	application/application.cpp
	application/backgroundthread.hpp
	application/backgroundthread.cpp
	application/settingsmanager.hpp
	application/settingsmanager.cpp)

set (SLICER_UI_SOURCES
     ui/aboutdialog.hpp
     ui/aboutdialog.cpp
     ui/actionbar.hpp
     ui/actionbar.cpp
	 ui/appwindow.hpp
	 ui/appwindow.cpp
	 ui/filechoosernative.hpp
	 ui/filechoosernative.cpp
     ui/headerbar.hpp
     ui/headerbar.cpp
	 ui/interactivepagewidget.hpp
	 ui/interactivepagewidget.cpp
     ui/openfiledialog.hpp
     ui/openfiledialog.cpp
	 ui/pagewidget.hpp
	 ui/pagewidget.cpp
	 ui/previewwindow.hpp
	 ui/previewwindow.cpp
     ui/savefiledialog.hpp
     ui/savefiledialog.cpp
	 ui/savingrevealer.hpp
	 ui/savingrevealer.cpp
     ui/utils.hpp
     ui/utils.cpp
     ui/view.hpp
     ui/view.cpp
     ui/welcomescreen.hpp
     ui/welcomescreen.cpp
     ui/zoomlevel.hpp
     ui/zoomlevel.cpp
     ui/zoomlevelwithactions.hpp
     ui/zoomlevelwithactions.cpp)

include (GResource)
gresource (${SOURCE_DATA_DIR} pdfslicer.gresource.xml
	 ${CMAKE_CURRENT_BINARY_DIR} GLIB_RESOURCE_FILE)

add_executable (${CMAKE_PROJECT_NAME}
				main.cpp
				${SLICER_APPLICATION_SOURCES}
                ${SLICER_BACKEND_SOURCES}
				${SLICER_UI_SOURCES}
				${GLIB_RESOURCE_FILE})
add_definitions ("-DGETTEXT_PACKAGE=\"${GETTEXT_PACKAGE}\"")

target_include_directories (
    ${CMAKE_PROJECT_NAME} SYSTEM PRIVATE
    ${CMAKE_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/third-party/adda25-threadpool
	${CMAKE_SOURCE_DIR}/third-party/spdlog/include
    ${CMAKE_SOURCE_DIR}/third-party/range-v3/include
	${CMAKE_CURRENT_SOURCE_DIR}/backend
	${CMAKE_CURRENT_SOURCE_DIR}/logger
)

if (MINGW)
    target_include_directories (${CMAKE_PROJECT_NAME} PRIVATE
		${GTKMM_INCLUDE_DIRS}
		${POPPLER_INCLUDE_DIRS})
else ()
    target_include_directories (${CMAKE_PROJECT_NAME} SYSTEM PRIVATE
		${GTKMM_INCLUDE_DIRS}
		${POPPLER_INCLUDE_DIRS})
endif ()

target_link_libraries (${CMAKE_PROJECT_NAME}
                       ${GTKMM_LDFLAGS}
                       ${INTL_LIBRARY_DIRS}
					   ThreadPool
					   backend
					   logger)

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

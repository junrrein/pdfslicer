set (SLICER_APPLICATION_SOURCES
	application/application.hpp
	application/application.cpp)

set (SLICER_BACKEND_SOURCES
     backend/command.hpp
     backend/command.cpp
     backend/commandmanager.hpp
     backend/commandmanager.cpp
     backend/document.hpp
     backend/document.cpp
     backend/page.hpp
     backend/page.cpp)

set (SLICER_UI_SOURCES
     ui/aboutdialog.hpp
     ui/aboutdialog.cpp
     ui/actionbar.hpp
     ui/actionbar.cpp
     ui/editor.hpp
     ui/editor.cpp
	 ui/filechoosernative.hpp
	 ui/filechoosernative.cpp
     ui/headerbar.hpp
     ui/headerbar.cpp
     ui/openfiledialog.hpp
     ui/openfiledialog.cpp
     ui/previewwindow.hpp
     ui/previewwindow.cpp
     ui/savefiledialog.hpp
     ui/savefiledialog.cpp
     ui/utils.hpp
     ui/utils.cpp
     ui/view.hpp
     ui/view.cpp
     ui/viewchild.hpp
     ui/viewchild.cpp
     ui/welcomescreen.hpp
     ui/welcomescreen.cpp
     ui/window.hpp
     ui/window.cpp
     ui/zoomlevel.hpp
     ui/zoomlevel.cpp
     ui/zoomlevelwithactions.hpp
     ui/zoomlevelwithactions.cpp)

add_executable (${CMAKE_PROJECT_NAME}
                main.cpp
				${SLICER_APPLICATION_SOURCES}
                ${SLICER_BACKEND_SOURCES}
                ${SLICER_UI_SOURCES})
add_definitions ("-DGETTEXT_PACKAGE=\"${GETTEXT_PACKAGE}\"")

target_include_directories (
    ${CMAKE_PROJECT_NAME} SYSTEM PRIVATE
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/third-party/PDF-Writer/PDFWriter
    ${CMAKE_SOURCE_DIR}/third-party/CTPL
    ${CMAKE_SOURCE_DIR}/third-party/range-v3/include
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
                       ${POPPLER_LDFLAGS}
                       ${INTL_LIBRARY_DIRS}
                       PDFWriter
                       CTPL)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set_target_properties (
            ${CMAKE_PROJECT_NAME}
            PROPERTIES
                COMPILE_FLAGS
                "-Wall -Wextra -Wpedantic -Wshadow -Wduplicated-cond -Wduplicated-branches -Wlogical-op"
        )
    endif ()

    # if (CLANG_TIDY_EXE) set_target_properties (${CMAKE_PROJECT_NAME}
    # PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}") endif ()

    # if (IWYU_EXE) set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES
    # CXX_INCLUDE_WHAT_YOU_USE "${DO_IWYU}") endif ()
endif ()

if (MINGW)
	target_sources (${CMAKE_PROJECT_NAME} PUBLIC pdfslicer.rc)

	if (CMAKE_BUILD_TYPE STREQUAL "Release")
		target_link_libraries (${PROJECT_NAME} "-mwindows")
	endif ()
endif ()

install (TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)

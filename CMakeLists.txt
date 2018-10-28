cmake_minimum_required (VERSION 3.7)
project (pdfslicer CXX C)

list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)

set (APPLICATION_ID "com.github.junrrein.PDFSlicer")
set (APPLICATION_NAME "PDF Slicer")
set (APPLICATION_WEBSITE "https://github.com/junrrein/pdfslicer")
set (GETTEXT_PACKAGE ${APPLICATION_ID})
set (LINUX_LOCALE_DIR "${CMAKE_INSTALL_PREFIX}/share/locale/")
set (VERSION 1.4.0)

set (SOURCE_DATA_DIR ${CMAKE_SOURCE_DIR}/data)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package (Threads REQUIRED)
find_package (PkgConfig REQUIRED)
pkg_check_modules (GTKMM gtkmm-3.0 IMPORTED_TARGET REQUIRED)
pkg_check_modules (POPPLER poppler-glib IMPORTED_TARGET REQUIRED)
find_package (Intl REQUIRED)
find_package (Gettext REQUIRED)

find_program (CLANG_TIDY_EXE
              NAMES "clang-tidy"
              DOC "Path to clang-tidy executable")
if (NOT CLANG_TIDY_EXE)
    message (STATUS "clang-tidy not found.")
else ()
    message (STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set (DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-config=")
endif ()

set (GNU_COMPILE_FLAGS "-Wall -Wextra -Wpedantic -Wshadow -Wduplicated-cond -Wduplicated-branches -Wlogical-op")

add_subdirectory (third-party)
add_subdirectory (src)
add_subdirectory (tests)
add_subdirectory (data)
add_subdirectory (po)

if (MINGW)
	include (WindowsInstaller)
endif ()

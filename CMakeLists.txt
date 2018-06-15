cmake_minimum_required (VERSION 3.7)
project (pdfslicer CXX)

list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)
set (APPLICATION_ID "com.github.junrrein.PDFSlicer")
set (APPLICATION_NAME "PDF Slicer")
set (APPLICATION_WEBSITE "https://github.com/junrrein/pdfslicer")
set (GETTEXT_PACKAGE ${APPLICATION_ID})
set (LOCALE_DIR "${CMAKE_INSTALL_PREFIX}/share/locale/")
set (VERSION 1.2.0)
configure_file (config.hpp.in ${CMAKE_BINARY_DIR}/config.hpp)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package (Threads REQUIRED)
find_package (PkgConfig REQUIRED)
pkg_check_modules (GTKMM gtkmm-3.0 REQUIRED)
pkg_check_modules (POPPLER poppler-glib REQUIRED)
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

find_program (IWYU_EXE
              NAMES "iwyu"
              DOC "Path to include-what-you-use executable")
if (NOT IWYU_EXE)
    message (STATUS "include-what-you-use not found.")
else ()
    message (STATUS "include-what-you-use found: ${IWYU_EXE}")
    set (DO_IWYU ${IWYU_EXE})
endif ()

add_subdirectory (third-party)

add_subdirectory (src)
add_subdirectory (data)
add_subdirectory (po)

# Windows installer
if (MINGW)
	include (WindowsInstaller)
endif ()

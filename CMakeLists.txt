cmake_minimum_required(VERSION 3.9)
project (pdfslicer CXX)

list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)
set (GETTEXT_PACKAGE "com.junrrein.pdfslicer")
set (GNOMELOCALEDIR "${CMAKE_INSTALL_PREFIX}/share/locale/")
set (VERSION 0.9)
configure_file (config.hpp.in ${CMAKE_BINARY_DIR}/config.hpp)

add_definitions ("-DGETTEXT_PACKAGE=\"${GETTEXT_PACKAGE}\"")

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package (Threads REQUIRED)
find_package (PkgConfig REQUIRED)
pkg_check_modules (GTKMM gtkmm-3.0 REQUIRED)
pkg_check_modules (POPPLER poppler-glib REQUIRED)
pkg_check_modules (CAIROMM cairomm-1.0 REQUIRED)
find_package (Intl REQUIRED)
find_package (Gettext REQUIRED)

find_program(
  CLANG_TIDY_EXE
  NAMES "clang-tidy"
  DOC "Path to clang-tidy executable"
  )
if(NOT CLANG_TIDY_EXE)
  message(STATUS "clang-tidy not found.")
else()
  message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
  set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-config=")
endif()

find_program(
  IWYU_EXE
  NAMES "iwyu"
  DOC "Path to include-what-you-use executable"
  )
if(NOT IWYU_EXE)
  message(STATUS "include-what-you-use not found.")
else()
  message(STATUS "include-what-you-use found: ${IWYU_EXE}")
  set(DO_IWYU ${IWYU_EXE})
endif()

include_directories (${GTKMM_INCLUDE_DIRS})
include_directories (${POPPLER_INCLUDE_DIRS})
include_directories (${CAIROMM_INCLUDE_DIRS})

add_subdirectory(third-party)

include (InstallRequiredSystemLibraries)
include (CPack)

add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(po)

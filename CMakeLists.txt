cmake_minimum_required(VERSION 3.9)

project(pdfslicer)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
find_package (Threads REQUIRED)
find_package (PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)
pkg_check_modules(POPPLER poppler-glib REQUIRED)
pkg_check_modules(CAIROMM cairomm-1.0 REQUIRED)

include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(${POPPLER_INCLUDE_DIRS})
include_directories(${CAIROMM_INCLUDE_DIRS})

# Don't use PDFWriter's bundled dependencies
# (Only a couple of them)
set(USE_BUNDLED "Off")

add_subdirectory(third-party)

include_directories (${CMAKE_SOURCE_DIR}/third-party/PDF-Writer/PDFWriter)
include_directories (${CMAKE_SOURCE_DIR}/third-party/CTPL)

add_subdirectory(src)
add_subdirectory(data)

cmake_minimum_required(VERSION 3.9)

project(pdfslicer)

find_package (Threads REQUIRED)
find_package (PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)
pkg_check_modules(POPPLER poppler-glib REQUIRED)
pkg_check_modules(CAIROMM cairomm-1.0 REQUIRED)

include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(${POPPLER_INCLUDE_DIRS})
include_directories(${CAIROMM_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 \
    -Wall -Wextra -Wpedantic -Wshadow -Wduplicated-cond -Wduplicated-branches -Wlogical-op\
    -lpng16")

# Don't use PDFWriter's bundled dependencies
set(USE_BUNDLED "Off")

add_subdirectory(PDF-Writer)
add_subdirectory(CTPL)

include_directories (${CMAKE_SOURCE_DIR}/PDF-Writer/PDFWriter)
include_directories (${CMAKE_SOURCE_DIR}/CTPL)

add_subdirectory(src)
add_subdirectory(data)

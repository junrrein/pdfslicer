cmake_minimum_required(VERSION 3.9)

project(pdfSlicer)

find_package (PkgConfig REQUIRED)
find_package (Threads REQUIRED)

pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)
pkg_check_modules(POPPLER poppler-glib REQUIRED)
pkg_check_modules(CAIROMM cairomm-1.0 REQUIRED)

link_directories(${GTKMM_LIBRARY_DIRS})
link_directories(${POPPLER_LIBRARY_DIRS})
link_directories(${CAIROMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(${POPPLER_INCLUDE_DIRS})
include_directories(${CAIROMM_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Wpedantic -Wshadow -lpng16")

# Check for PDFWriter's dependencies
PKG_CHECK_MODULES(LIBJPEG libjpeg REQUIRED)
PKG_CHECK_MODULES(LIBTIFF libtiff-4 REQUIRED)
PKG_CHECK_MODULES(LINPNG libpng REQUIRED)

# Don't use PDFWriter's bundled dependencies
set(USE_BUNDLED OFF)

add_subdirectory(PDF-Writer)
add_subdirectory(CTPL)

include_directories (${CMAKE_SOURCE_DIR}/PDF-Writer/PDFWriter)
include_directories (${CMAKE_SOURCE_DIR}/CTPL)

add_subdirectory(src)

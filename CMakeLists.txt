cmake_minimum_required(VERSION 3.9)

project(poppler-stuff)

find_package(PkgConfig)

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

add_subdirectory(PDF-Writer)

configure_file(config.hpp.cmake ${CMAKE_SOURCE_DIR}/config.hpp)

add_executable(poppler-stuff
    main.cpp
    config.hpp
    slicerdocument.hpp
    slicerdocument.cpp
    slicerview.hpp
    slicerview.cpp
    previewwindow.hpp
    previewwindow.cpp
    slicerwindow.hpp
    slicerwindow.cpp)
target_link_libraries(poppler-stuff
    ${GTKMM_LIBRARIES}
    ${CAIROMM_LIBRARIES}
    ${POPPLER_LIBRARIES}
    PDFWriter)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lti2.pdf
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

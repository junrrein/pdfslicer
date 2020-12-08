cmake_minimum_required (VERSION 3.7)
project (pdfslicer CXX C)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (APPLICATION_ID "com.github.junrrein.PDFSlicer")
set (APPLICATION_NAME "PDF Slicer")
set (APPLICATION_WEBSITE "https://junrrein.github.io/pdfslicer/")
set (GETTEXT_PACKAGE ${APPLICATION_ID})
set (LINUX_LOCALE_DIR "${CMAKE_INSTALL_PREFIX}/share/locale/")
set (VERSION 1.8.8)

set (SOURCE_DATA_DIR ${CMAKE_SOURCE_DIR}/data)

find_package (Threads REQUIRED)
find_package (PkgConfig REQUIRED)
pkg_check_modules (GTKMM gtkmm-3.0 IMPORTED_TARGET REQUIRED)
pkg_check_modules (POPPLER poppler-cpp IMPORTED_TARGET REQUIRED)
find_package (Intl REQUIRED)
find_package (Gettext REQUIRED)

if (MINGW)
	include (FetchContent)
	FetchContent_Declare(
		qpdf
		GIT_REPOSITORY https://github.com/qpdf/qpdf.git
		GIT_TAG	release-qpdf-10.0.4)

	FetchContent_GetProperties(qpdf)
	if (NOT qpdf_POPULATED)
		FetchContent_Populate (qpdf)

		message ("Configuring and building QPDF")
		execute_process (COMMAND ./configure --host=x86_64-w64-mingw32
						 WORKING_DIRECTORY ${qpdf_SOURCE_DIR})
		execute_process (COMMAND make -j4
						 WORKING_DIRECTORY ${qpdf_SOURCE_DIR})

		add_library (qpdf_lib SHARED IMPORTED)
		set_target_properties (qpdf_lib PROPERTIES
							   IMPORTED_LOCATION "${qpdf_SOURCE_DIR}/libqpdf/build/.libs/libqpdf-28.dll"
							   IMPORTED_IMPLIB "${qpdf_SOURCE_DIR}/libqpdf/build/.libs/libqpdf.dll.a")
		target_include_directories (qpdf_lib INTERFACE ${qpdf_SOURCE_DIR}/include/)
	endif ()
else ()
	# See https://unix.stackexchange.com/q/362289/
	set (ENV{PKG_CONFIG_ALLOW_SYSTEM_CFLAGS} 1)
	pkg_check_modules (QPDF libqpdf IMPORTED_TARGET REQUIRED)
	set (ENV{PKG_CONFIG_ALLOW_SYSTEM_CFLAGS} 0)
endif ()

set (SLICER_DEBUG_FLAGS -Wall -Wextra -Wpedantic -Wshadow -Wduplicated-cond -Wduplicated-branches -Wlogical-op)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)
include (Utils)
add_subdirectory (third-party)
add_subdirectory (src)
add_subdirectory (tests)
add_subdirectory (data)
add_subdirectory (po)

if (MINGW)
	include (WindowsInstaller)
endif ()

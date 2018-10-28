set (SOURCES
	 command.hpp
	 command.cpp
	 commandmanager.hpp
	 commandmanager.cpp
	 document.hpp
	 document.cpp
	 page.hpp
	 page.cpp
	 pdfsaver.hpp
	 pdfsaver.cpp
	 pagerenderer.hpp
	 pagerenderer.cpp
	 tempfile.hpp
	 tempfile.cpp)

add_library (backend STATIC ${SOURCES})

target_include_directories (backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories (
	backend SYSTEM PUBLIC
	${CMAKE_SOURCE_DIR}/third-party/PDF-Writer/PDFWriter
)

target_link_libraries (backend
					   config
					   PDFWriter
					   stduuid
					   PkgConfig::GTKMM
					   PkgConfig::POPPLER)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
   if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	   set_target_properties (backend PROPERTIES COMPILE_FLAGS ${GNU_COMPILE_FLAGS})
   endif ()

#	 if (CLANG_TIDY_EXE)
#		 set_target_properties (backend PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
#	 endif ()
endif ()

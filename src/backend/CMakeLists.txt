set (SOURCES
	 command.hpp
	 command.cpp
	 commandmanager.hpp
	 commandmanager.cpp
	 document.hpp
	 document.cpp
	 documentsaver.hpp
	 documentsaver.cpp
	 page.hpp
	 page.cpp
	 pagerenderer.hpp
	 pagerenderer.cpp
	 tempfile.hpp
	 tempfile.cpp)

add_library (backend STATIC ${SOURCES})

target_include_directories (
	backend SYSTEM PRIVATE
	${CMAKE_SOURCE_DIR}/third-party/PDF-Writer/PDFWriter
	${CMAKE_SOURCE_DIR}/third-party/stduuid
)

if (MINGW)
	target_include_directories (backend PRIVATE
		${GTKMM_INCLUDE_DIRS}
		${POPPLER_INCLUDE_DIRS})
else ()
	target_include_directories (backend SYSTEM PRIVATE
		${GTKMM_INCLUDE_DIRS}
		${POPPLER_INCLUDE_DIRS})
endif ()

target_link_libraries (backend
					   config
					   ${GTKMM_LDFLAGS}
					   ${POPPLER_LDFLAGS}
					   PDFWriter
					   stduuid)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
   if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	   set_target_properties (backend PROPERTIES COMPILE_FLAGS ${GNU_COMPILE_FLAGS})
   endif ()

#	 if (CLANG_TIDY_EXE)
#		 set_target_properties (backend PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
#	 endif ()
endif ()

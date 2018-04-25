set (SLICER_BACKEND_SOURCES
	backend/command.hpp
	backend/command.cpp
	backend/commandmanager.hpp
	backend/commandmanager.cpp
	backend/document.hpp
	backend/document.cpp
	backend/page.hpp
	backend/page.cpp)

set (SLICER_UI_SOURCES
	ui/aboutdialog.hpp
	ui/aboutdialog.cpp
	ui/actionbar.hpp
	ui/actionbar.cpp
	ui/editor.hpp
	ui/editor.cpp
	ui/headerbar.hpp
	ui/headerbar.cpp
	ui/openfiledialog.hpp
	ui/openfiledialog.cpp
	ui/previewwindow.hpp
	ui/previewwindow.cpp
	ui/savefiledialog.hpp
	ui/savefiledialog.cpp
	ui/utils.hpp
	ui/utils.cpp
	ui/view.hpp
	ui/view.cpp
	ui/viewchild.hpp
	ui/viewchild.cpp
	ui/welcomescreen.hpp
	ui/welcomescreen.cpp
	ui/window.hpp
	ui/window.cpp
	ui/zoomlevel.hpp
	ui/zoomlevel.cpp
	ui/zoomlevelwithactions.hpp
	ui/zoomlevelwithactions.cpp)

add_executable(pdfslicer
    main.cpp
	application.hpp
	application.cpp
	${SLICER_BACKEND_SOURCES}
	${SLICER_UI_SOURCES})

target_include_directories (pdfslicer SYSTEM PRIVATE
	"${CMAKE_BINARY_DIR}"
	"${CMAKE_SOURCE_DIR}/third-party/PDF-Writer/PDFWriter"
	"${CMAKE_SOURCE_DIR}/third-party/CTPL"
	"${CMAKE_SOURCE_DIR}/third-party/range-v3/include")

target_link_libraries(pdfslicer
    ${GTKMM_LIBRARIES}
    ${CAIROMM_LIBRARIES}
    ${POPPLER_LIBRARIES}
	${INTL_LIBRARY_DIRS}
    PDFWriter
	CTPL)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set_target_properties(pdfslicer PROPERTIES COMPILE_FLAGS
			"-Wall -Wextra -Wpedantic -Wshadow -Wduplicated-cond -Wduplicated-branches -Wlogical-op")
	endif()

#	if(CLANG_TIDY_EXE)
#	  set_target_properties(pdfslicer PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
#	endif()

#	if (IWYU_EXE)
#		set_target_properties(pdfslicer PROPERTIES CXX_INCLUDE_WHAT_YOU_USE "${DO_IWYU}")
#	endif ()
endif()

if (MINGW)
	set (PDFWRITER_DLL "${CMAKE_BINARY_DIR}/third-party/PDF-Writer/PDFWriter/libPDFWriter.dll")
	set (PDFWRITER_AESGM_DLL "${CMAKE_BINARY_DIR}/third-party/PDF-Writer/LibAesgm/libLibAesgm.dll")
	set (PDFWRITER_TIFF_DLL "${CMAKE_BINARY_DIR}/third-party/PDF-Writer/LibTiff/libLibTiff.dll")

	set (FEDORA_MINGW_LIBBINPATH "/usr/x86_64-w64-mingw32/sys-root/mingw/bin")

	set (ATK_DLL "${FEDORA_MINGW_LIBBINPATH}/libatk-1.0-0.dll")
	set (ATKMM_DLL "${FEDORA_MINGW_LIBBINPATH}/libatkmm-1.6-1.dll")
	set (CAIRO_DLL "${FEDORA_MINGW_LIBBINPATH}/libcairo-2.dll")
	set (CAIROMM_DLL "${FEDORA_MINGW_LIBBINPATH}/libcairomm-1.0-1.dll")
	set (FFI_DLL "${FEDORA_MINGW_LIBBINPATH}/libffi-6.dll")
	set (GCC_DLL "${FEDORA_MINGW_LIBBINPATH}/libgcc_s_seh-1.dll")
	set (GDKMM_DLL "${FEDORA_MINGW_LIBBINPATH}/libgdkmm-3.0-1.dll")
	set (GIO_DLL "${FEDORA_MINGW_LIBBINPATH}/libgio-2.0-0.dll")
	set (GIOMM_DLL "${FEDORA_MINGW_LIBBINPATH}/libgiomm-2.4-1.dll")
	set (GLIB_DLL "${FEDORA_MINGW_LIBBINPATH}/libglib-2.0-0.dll")
	set (GLIBMM_DLL "${FEDORA_MINGW_LIBBINPATH}/libglibmm-2.4-1.dll")
	set (GMODULE_DLL "${FEDORA_MINGW_LIBBINPATH}/libgmodule-2.0-0.dll")
	set (GOBJECT_DLL "${FEDORA_MINGW_LIBBINPATH}/libgobject-2.0-0.dll")
	set (GTK_DLL "${FEDORA_MINGW_LIBBINPATH}/libgtk-3-0.dll")
	set (GTKMM_DLL "${FEDORA_MINGW_LIBBINPATH}/libgtkmm-3.0-1.dll")
	set (POPPLER_GLIB_DLL "${FEDORA_MINGW_LIBBINPATH}/libpoppler-glib-8.dll")
	set (WINPTHREAD_DLL "${FEDORA_MINGW_LIBBINPATH}/libwinpthread-1.dll")
	set (SIGC_DLL "${FEDORA_MINGW_LIBBINPATH}/libsigc-2.0-0.dll")
	set (CPP_RUNTIME_DLL "${FEDORA_MINGW_LIBBINPATH}/libstdc++-6.dll")
	set (INTL_DLL "${FEDORA_MINGW_LIBBINPATH}/libintl-8.dll")
	set (PCRE_DLL "${FEDORA_MINGW_LIBBINPATH}/libpcre-1.dll")
	set (GDK_DLL "${FEDORA_MINGW_LIBBINPATH}/libgdk-3-0.dll")
	set (GDK_PIXBUF_DLL "${FEDORA_MINGW_LIBBINPATH}/libgdk_pixbuf-2.0-0.dll")
	set (ZLIB_DLL "${FEDORA_MINGW_LIBBINPATH}/zlib1.dll")
	set (CAIRO_GOBJECT_DLL "${FEDORA_MINGW_LIBBINPATH}/libcairo-gobject-2.dll")
	set (EPOXY_DLL "${FEDORA_MINGW_LIBBINPATH}/libepoxy-0.dll")
	set (PANGO_DLL "${FEDORA_MINGW_LIBBINPATH}/libpango-1.0-0.dll")
	set (PANGOCAIRO_DLL "${FEDORA_MINGW_LIBBINPATH}/libpangocairo-1.0-0.dll")
	set (PANGOMM_DLL "${FEDORA_MINGW_LIBBINPATH}/libpangomm-1.4-1.dll")
	set (POPPLER_DLL "${FEDORA_MINGW_LIBBINPATH}/libpoppler-68.dll")
	set (FREETYPE_DLL "${FEDORA_MINGW_LIBBINPATH}/libfreetype-6.dll")
	set (ICONV_DLL "${FEDORA_MINGW_LIBBINPATH}/iconv.dll")
	set (PIXMAN_DLL "${FEDORA_MINGW_LIBBINPATH}/libpixman-1-0.dll")
	set (PNG_DLL "${FEDORA_MINGW_LIBBINPATH}/libpng16-16.dll")
	set (CAIRO_GOBJECT_DLL "${FEDORA_MINGW_LIBBINPATH}/libcairo-gobject-2.dll")
	set (PANGO_WIN32_DLL "${FEDORA_MINGW_LIBBINPATH}/libpangowin32-1.0-0.dll")
	set (FONTCONFIG_DLL "${FEDORA_MINGW_LIBBINPATH}/libfontconfig-1.dll")
	set (BZ2_DLL "${FEDORA_MINGW_LIBBINPATH}/libbz2-1.dll")
	set (JPEG_DLL "${FEDORA_MINGW_LIBBINPATH}/libjpeg-62.dll")
	set (LCMS2_DLL "${FEDORA_MINGW_LIBBINPATH}/liblcms2-2.dll")
	set (OPENJP2_DLL "${FEDORA_MINGW_LIBBINPATH}/libopenjp2.dll")
	set (TIFF_DLL "${FEDORA_MINGW_LIBBINPATH}/libtiff-5.dll")
	set (EXPAT_DLL "${FEDORA_MINGW_LIBBINPATH}/libexpat-1.dll")

	set (SLICER_DLLS
		${PDFWRITER_DLL}
		${PDFWRITER_AESGM_DLL};
		${PDFWRITER_TIFF_DLL};
		${ATK_DLL};
		${ATKMM_DLL};
		${CAIRO_DLL};
		${CAIROMM_DLL};
		${FFI_DLL};
		${GCC_DLL};
		${GDKMM_DLL};
		${GIO_DLL};
		${GIOMM_DLL};
		${GLIB_DLL};
		${GLIBMM_DLL};
		${GMODULE_DLL};
		${GOBJECT_DLL};
		${GTK_DLL};
		${GTKMM_DLL};
		${POPPLER_GLIB_DLL};
		${WINPTHREAD_DLL};
		${SIGC_DLL};
		${CPP_RUNTIME_DLL};
		${INTL_DLL};
		${PCRE_DLL};
		${GDK_DLL};
		${GDK_PIXBUF_DLL};
		${ZLIB_DLL};
		${CAIRO_GOBJECT_DLL};
		${EPOXY_DLL};
		${PANGO_DLL};
		${PANGOCAIRO_DLL};
		${PANGOMM_DLL};
		${POPPLER_DLL};
		${FREETYPE_DLL};
		${ICONV_DLL};
		${PIXMAN_DLL};
		${PNG_DLL};
		${CAIRO_GOBJECT_DLL};
		${PANGO_WIN32_DLL};
		${FONTCONFIG_DLL};
		${BZ2_DLL};
		${JPEG_DLL};
		${LCMS2_DLL};
		${OPENJP2_DLL};
		${TIFF_DLL};
		${EXPAT_DLL})

	set (GTK_STUFF_DIR "/usr/x86_64-w64-mingw32/sys-root/mingw/share")

	install (PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/pdfslicer.exe DESTINATION ./)
	install (FILES ${SLICER_DLLS} DESTINATION ./)
	install (DIRECTORY ${GTK_STUFF_DIR} DESTINATION ./)
endif()

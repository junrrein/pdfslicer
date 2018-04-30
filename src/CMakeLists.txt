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

add_executable (${CMAKE_PROJECT_NAME}
                main.cpp
                application.hpp
                application.cpp
                ${SLICER_BACKEND_SOURCES}
                ${SLICER_UI_SOURCES})
add_definitions ("-DGETTEXT_PACKAGE=\"${GETTEXT_PACKAGE}\"")

target_include_directories (
    ${CMAKE_PROJECT_NAME}
    SYSTEM
    PRIVATE
    "${CMAKE_BINARY_DIR}"
    "${CMAKE_SOURCE_DIR}/third-party/PDF-Writer/PDFWriter"
    "${CMAKE_SOURCE_DIR}/third-party/CTPL"
    "${CMAKE_SOURCE_DIR}/third-party/range-v3/include"
)

target_link_libraries (${CMAKE_PROJECT_NAME}
                       ${GTKMM_LIBRARIES}
                       ${CAIROMM_LIBRARIES}
                       ${POPPLER_LIBRARIES}
                       ${INTL_LIBRARY_DIRS}
                       PDFWriter
                       CTPL)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set_target_properties (
            ${CMAKE_PROJECT_NAME}
            PROPERTIES
                COMPILE_FLAGS
                "-Wall -Wextra -Wpedantic -Wshadow -Wduplicated-cond -Wduplicated-branches -Wlogical-op"
        )
    endif ()

    # if (CLANG_TIDY_EXE) set_target_properties (${CMAKE_PROJECT_NAME}
    # PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}") endif ()

    # if (IWYU_EXE) set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES
    # CXX_INCLUDE_WHAT_YOU_USE "${DO_IWYU}") endif ()
endif ()

if (MINGW)
	target_link_libraries (${PROJECT_NAME} "-mwindows")

	set (CPACK_PACKAGE_NAME ${APPLICATION_NAME})
	set (CPACK_PACKAGE_VENDOR "Julián Unrrein")
	set (CPACK_PACKAGE_VERSION ${VERSION})
	set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
	set (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
	set (CPACK_PACKAGE_EXECUTABLES ${CMAKE_PROJECT_NAME} ${CPACK_PACKAGE_NAME})
	set (CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
	set (CPACK_VERBATIM_VARIABLES TRUE)

	set (CPACK_GENERATOR "NSIS")
	set (CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL TRUE)
	set (CPACK_NSIS_URL_INFO_ABOUT ${APPLICATION_WEBSITE})
	set (CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
	set (CPACK_NSIS_MUI_FINISHPAGE_RUN ${CMAKE_PROJECT_NAME})

	include (CPack)

    set (PDFWRITER_DLL
         "${CMAKE_BINARY_DIR}/third-party/PDF-Writer/PDFWriter/libPDFWriter.dll"
         )
    set (PDFWRITER_AESGM_DLL
         "${CMAKE_BINARY_DIR}/third-party/PDF-Writer/LibAesgm/libLibAesgm.dll")
    set (PDFWRITER_TIFF_DLL
         "${CMAKE_BINARY_DIR}/third-party/PDF-Writer/LibTiff/libLibTiff.dll")

    set (FEDORA_MINGW_LIBBINPATH "/usr/x86_64-w64-mingw32/sys-root/mingw/bin")

	file (GLOB ATK_DLL "${FEDORA_MINGW_LIBBINPATH}/libatk*.dll")
	file (GLOB ATKMM_DLL "${FEDORA_MINGW_LIBBINPATH}/libatkmm*.dll")
	file (GLOB CAIRO_DLL "${FEDORA_MINGW_LIBBINPATH}/libcairo*.dll")
	file (GLOB CAIROMM_DLL "${FEDORA_MINGW_LIBBINPATH}/libcairomm*.dll")
	file (GLOB FFI_DLL "${FEDORA_MINGW_LIBBINPATH}/libffi*.dll")
	file (GLOB GCC_DLL "${FEDORA_MINGW_LIBBINPATH}/libgcc*.dll")
	file (GLOB GDKMM_DLL "${FEDORA_MINGW_LIBBINPATH}/libgdkmm*.dll")
	file (GLOB GIO_DLL "${FEDORA_MINGW_LIBBINPATH}/libgio*.dll")
	file (GLOB GIOMM_DLL "${FEDORA_MINGW_LIBBINPATH}/libgiomm*.dll")
	file (GLOB GLIB_DLL "${FEDORA_MINGW_LIBBINPATH}/libglib*.dll")
	file (GLOB GLIBMM_DLL "${FEDORA_MINGW_LIBBINPATH}/libglibmm*.dll")
	file (GLOB GMODULE_DLL "${FEDORA_MINGW_LIBBINPATH}/libgmodule*.dll")
	file (GLOB GOBJECT_DLL "${FEDORA_MINGW_LIBBINPATH}/libgobject*.dll")
	file (GLOB GTK_DLL "${FEDORA_MINGW_LIBBINPATH}/libgtk*.dll")
	file (GLOB GTKMM_DLL "${FEDORA_MINGW_LIBBINPATH}/libgtkmm*.dll")
	file (GLOB POPPLER_GLIB_DLL "${FEDORA_MINGW_LIBBINPATH}/libpoppler-glib*.dll")
	file (GLOB WINPTHREAD_DLL "${FEDORA_MINGW_LIBBINPATH}/libwinpthread*.dll")
	file (GLOB SIGC_DLL "${FEDORA_MINGW_LIBBINPATH}/libsigc*.dll")
	file (GLOB CPP_RUNTIME_DLL "${FEDORA_MINGW_LIBBINPATH}/libstdc++*.dll")
	file (GLOB INTL_DLL "${FEDORA_MINGW_LIBBINPATH}/libintl*.dll")
	file (GLOB PCRE_DLL "${FEDORA_MINGW_LIBBINPATH}/libpcre*.dll")
	file (GLOB GDK_DLL "${FEDORA_MINGW_LIBBINPATH}/libgdk*.dll")
	file (GLOB GDK_PIXBUF_DLL "${FEDORA_MINGW_LIBBINPATH}/libgdk_pixbuf*.dll")
	file (GLOB ZLIB_DLL "${FEDORA_MINGW_LIBBINPATH}/zlib*.dll")
	file (GLOB CAIRO_GOBJECT_DLL "${FEDORA_MINGW_LIBBINPATH}/libcairo-gobject*.dll")
	file (GLOB EPOXY_DLL "${FEDORA_MINGW_LIBBINPATH}/libepoxy*.dll")
	file (GLOB PANGO_DLL "${FEDORA_MINGW_LIBBINPATH}/libpango*.dll")
	file (GLOB PANGOCAIRO_DLL "${FEDORA_MINGW_LIBBINPATH}/libpangocairo*.dll")
	file (GLOB PANGOMM_DLL "${FEDORA_MINGW_LIBBINPATH}/libpangomm*.dll")
	file (GLOB POPPLER_DLL "${FEDORA_MINGW_LIBBINPATH}/libpoppler*.dll")
	file (GLOB FREETYPE_DLL "${FEDORA_MINGW_LIBBINPATH}/libfreetype*.dll")
	file (GLOB ICONV_DLL "${FEDORA_MINGW_LIBBINPATH}/iconv*.dll")
	file (GLOB PIXMAN_DLL "${FEDORA_MINGW_LIBBINPATH}/libpixman*.dll")
	file (GLOB PNG_DLL "${FEDORA_MINGW_LIBBINPATH}/libpng*.dll")
	file (GLOB CAIRO_GOBJECT_DLL "${FEDORA_MINGW_LIBBINPATH}/libcairo-gobject*.dll")
	file (GLOB PANGO_WIN32_DLL "${FEDORA_MINGW_LIBBINPATH}/libpangowin32*.dll")
	file (GLOB FONTCONFIG_DLL "${FEDORA_MINGW_LIBBINPATH}/libfontconfig*.dll")
	file (GLOB BZ2_DLL "${FEDORA_MINGW_LIBBINPATH}/libbz2*.dll")
	file (GLOB JPEG_DLL "${FEDORA_MINGW_LIBBINPATH}/libjpeg*.dll")
	file (GLOB LCMS2_DLL "${FEDORA_MINGW_LIBBINPATH}/liblcms2*.dll")
	file (GLOB OPENJP2_DLL "${FEDORA_MINGW_LIBBINPATH}/libopenjp*.dll")
	file (GLOB TIFF_DLL "${FEDORA_MINGW_LIBBINPATH}/libtiff*.dll")
	file (GLOB EXPAT_DLL "${FEDORA_MINGW_LIBBINPATH}/libexpat*.dll")

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

	install (TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ./)
    install (FILES ${SLICER_DLLS} DESTINATION ./)
	install (DIRECTORY ${GTK_STUFF_DIR} DESTINATION ./)

else () # Else we are in Linux
    install (TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)
endif ()

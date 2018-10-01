set (PDFWRITER_AESGM_DLL
     "${CMAKE_BINARY_DIR}/third-party/PDF-Writer/LibAesgm/libLibAesgm.dll")

set (FEDORA_MINGW_BASEPATH "/usr/x86_64-w64-mingw32/sys-root/mingw")
set (FEDORA_MINGW_BINPATH "${FEDORA_MINGW_BASEPATH}/bin")

file (GLOB ATK_DLL "${FEDORA_MINGW_BINPATH}/libatk*.dll")
file (GLOB ATKMM_DLL "${FEDORA_MINGW_BINPATH}/libatkmm*.dll")
file (GLOB CAIRO_DLL "${FEDORA_MINGW_BINPATH}/libcairo*.dll")
file (GLOB CAIROMM_DLL "${FEDORA_MINGW_BINPATH}/libcairomm*.dll")
file (GLOB FFI_DLL "${FEDORA_MINGW_BINPATH}/libffi*.dll")
file (GLOB GCC_DLL "${FEDORA_MINGW_BINPATH}/libgcc*.dll")
file (GLOB GDKMM_DLL "${FEDORA_MINGW_BINPATH}/libgdkmm*.dll")
file (GLOB GIO_DLL "${FEDORA_MINGW_BINPATH}/libgio*.dll")
file (GLOB GIOMM_DLL "${FEDORA_MINGW_BINPATH}/libgiomm*.dll")
file (GLOB GLIB_DLL "${FEDORA_MINGW_BINPATH}/libglib*.dll")
file (GLOB GLIBMM_DLL "${FEDORA_MINGW_BINPATH}/libglibmm*.dll")
file (GLOB GMODULE_DLL "${FEDORA_MINGW_BINPATH}/libgmodule*.dll")
file (GLOB GOBJECT_DLL "${FEDORA_MINGW_BINPATH}/libgobject*.dll")
file (GLOB GTK_DLL "${FEDORA_MINGW_BINPATH}/libgtk*.dll")
file (GLOB GTKMM_DLL "${FEDORA_MINGW_BINPATH}/libgtkmm*.dll")
file (GLOB POPPLER_GLIB_DLL "${FEDORA_MINGW_BINPATH}/libpoppler-glib*.dll")
file (GLOB WINPTHREAD_DLL "${FEDORA_MINGW_BINPATH}/libwinpthread*.dll")
file (GLOB SIGC_DLL "${FEDORA_MINGW_BINPATH}/libsigc*.dll")
file (GLOB CPP_RUNTIME_DLL "${FEDORA_MINGW_BINPATH}/libstdc++*.dll")
file (GLOB INTL_DLL "${FEDORA_MINGW_BINPATH}/libintl*.dll")
file (GLOB PCRE_DLL "${FEDORA_MINGW_BINPATH}/libpcre*.dll")
file (GLOB GDK_DLL "${FEDORA_MINGW_BINPATH}/libgdk*.dll")
file (GLOB GDK_PIXBUF_DLL "${FEDORA_MINGW_BINPATH}/libgdk_pixbuf*.dll")
file (GLOB ZLIB_DLL "${FEDORA_MINGW_BINPATH}/zlib*.dll")
file (GLOB CAIRO_GOBJECT_DLL "${FEDORA_MINGW_BINPATH}/libcairo-gobject*.dll")
file (GLOB EPOXY_DLL "${FEDORA_MINGW_BINPATH}/libepoxy*.dll")
file (GLOB PANGO_DLL "${FEDORA_MINGW_BINPATH}/libpango*.dll")
file (GLOB PANGOCAIRO_DLL "${FEDORA_MINGW_BINPATH}/libpangocairo*.dll")
file (GLOB PANGOMM_DLL "${FEDORA_MINGW_BINPATH}/libpangomm*.dll")
file (GLOB POPPLER_DLL "${FEDORA_MINGW_BINPATH}/libpoppler*.dll")
file (GLOB FREETYPE_DLL "${FEDORA_MINGW_BINPATH}/libfreetype*.dll")
file (GLOB ICONV_DLL "${FEDORA_MINGW_BINPATH}/iconv*.dll")
file (GLOB PIXMAN_DLL "${FEDORA_MINGW_BINPATH}/libpixman*.dll")
file (GLOB PNG_DLL "${FEDORA_MINGW_BINPATH}/libpng*.dll")
file (GLOB CAIRO_GOBJECT_DLL "${FEDORA_MINGW_BINPATH}/libcairo-gobject*.dll")
file (GLOB PANGO_WIN32_DLL "${FEDORA_MINGW_BINPATH}/libpangowin32*.dll")
file (GLOB FONTCONFIG_DLL "${FEDORA_MINGW_BINPATH}/libfontconfig*.dll")
file (GLOB BZ2_DLL "${FEDORA_MINGW_BINPATH}/libbz2*.dll")
file (GLOB JPEG_DLL "${FEDORA_MINGW_BINPATH}/libjpeg*.dll")
file (GLOB LCMS2_DLL "${FEDORA_MINGW_BINPATH}/liblcms2*.dll")
file (GLOB OPENJP2_DLL "${FEDORA_MINGW_BINPATH}/libopenjp*.dll")
file (GLOB TIFF_DLL "${FEDORA_MINGW_BINPATH}/libtiff*.dll")
file (GLOB EXPAT_DLL "${FEDORA_MINGW_BINPATH}/libexpat*.dll")
file (GLOB RSVG_DLL "${FEDORA_MINGW_BINPATH}/librsvg*.dll")
file (GLOB CROCO_DLL "${FEDORA_MINGW_BINPATH}/libcroco*.dll")
file (GLOB XML2_DLL "${FEDORA_MINGW_BINPATH}/libxml2*.dll")
file (GLOB HARFBUZZ_DLL "${FEDORA_MINGW_BINPATH}/libharfbuzz*.dll")

set (SLICER_DLLS
     ${PDFWRITER_AESGM_DLL};
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
     ${EXPAT_DLL};
     ${RSVG_DLL};
     ${CROCO_DLL};
     ${XML2_DLL};
     ${HARFBUZZ_DLL})

install (FILES ${SLICER_DLLS} DESTINATION bin)

set (GTK_SHARE_DIR "${FEDORA_MINGW_BASEPATH}/share")
install (DIRECTORY ${GTK_SHARE_DIR} DESTINATION ./)

set (GDK_PIXBUFLOADERS_RELDIR "lib/gdk-pixbuf-2.0/2.10.0/loaders")
set (
    GDK_SVGLOADER_DLL
    "${FEDORA_MINGW_BASEPATH}/${GDK_PIXBUFLOADERS_RELDIR}/libpixbufloader-svg.dll"
)
install (FILES ${GDK_SVGLOADER_DLL} DESTINATION ${GDK_PIXBUFLOADERS_RELDIR})

set (GDK_QUERY_LOADERS_EXE
     "${FEDORA_MINGW_BINPATH}/gdk-pixbuf-query-loaders.exe")
set (GLIB_COMPILE_SCHEMAS_EXE
     "${FEDORA_MINGW_BINPATH}/glib-compile-schemas.exe")
set (GSETTINGS_COMPILE OFF)
install (FILES ${GDK_QUERY_LOADERS_EXE}
               ${GLIB_COMPILE_SCHEMAS_EXE}
               DESTINATION
               bin)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set (GDB_EXE "${FEDORA_MINGW_BINPATH}/gdb.exe")
    install (FILES ${GDB_EXE} DESTINATION bin)
endif ()

set (CPACK_PACKAGE_NAME ${APPLICATION_NAME})
set (CPACK_PACKAGE_VENDOR "Julián Unrrein")
set (CPACK_PACKAGE_VERSION ${VERSION})
set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}")
set (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
set (CPACK_PACKAGE_EXECUTABLES ${CMAKE_PROJECT_NAME} ${CPACK_PACKAGE_NAME})
set (CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set (CPACK_MONOLITHIC_INSTALL TRUE)
set (CPACK_VERBATIM_VARIABLES TRUE)

set (CPACK_GENERATOR "NSIS")
set (CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL TRUE)
set (CPACK_NSIS_URL_INFO_ABOUT ${APPLICATION_WEBSITE})
set (CPACK_NSIS_MUI_FINISHPAGE_RUN ${CMAKE_PROJECT_NAME})
set (CPACK_NSIS_MUI_ICON ${CMAKE_BINARY_DIR}/pdfslicer.ico)
set (
    CPACK_NSIS_EXTRA_INSTALL_COMMANDS
    "ExecWait '\"$INSTDIR\\bin\\gdk-pixbuf-query-loaders\" --update-cache'\n
	 ExecWait '\"$INSTDIR\\bin\\glib-compile-schemas\" \"$INSTDIR\\share\\glib-2.0\\schemas\"'"
)
set (CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "RMDir /r \"$INSTDIR\"")

include (CPackComponent)
include (CPack)

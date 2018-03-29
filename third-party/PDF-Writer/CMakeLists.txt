project(PDFHUMMUS)
cmake_minimum_required (VERSION 2.6)

find_package (JPEG REQUIRED)
find_package (ZLIB REQUIRED)
find_package (Freetype REQUIRED)
find_package (PNG REQUIRED)

ADD_SUBDIRECTORY(LibTiff)

if (MINGW)
	ADD_SUBDIRECTORY(LibAesgm)
else()
	FIND_PATH(LIBAESGM_INCLUDE_DIRS aescpp.h /usr/local/include/aes /usr/include/aes)
	FIND_LIBRARY(LIBAESGM_LDFLAGS NAMES aesgm PATHS /usr/local/lib${LIB_SUFFIX} /usr/lib${LIB_SUFFIX})
endif()

ADD_SUBDIRECTORY(PDFWriter)

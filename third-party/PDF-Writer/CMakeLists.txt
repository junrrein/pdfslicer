project(PDFHUMMUS)
cmake_minimum_required (VERSION 2.6)

find_package (JPEG REQUIRED)
find_package (ZLIB REQUIRED)
find_package (Freetype REQUIRED)
find_package (PNG REQUIRED)

ADD_SUBDIRECTORY(LibTiff)
ADD_SUBDIRECTORY(LibAesgm)

ADD_SUBDIRECTORY(PDFWriter)

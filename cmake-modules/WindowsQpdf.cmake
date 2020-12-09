include (FetchContent)
FetchContent_Declare(
	qpdf
	GIT_REPOSITORY https://github.com/qpdf/qpdf.git
	GIT_TAG	release-qpdf-10.0.4)

FetchContent_GetProperties(qpdf)
if (NOT qpdf_POPULATED)
	FetchContent_Populate (qpdf)

	set (QPDF_DLL "${qpdf_SOURCE_DIR}/libqpdf/build/.libs/libqpdf-28.dll")
	set (QPDF_IMPLIB "${qpdf_SOURCE_DIR}/libqpdf/build/.libs/libqpdf.dll.a")

	add_custom_command (OUTPUT ${QPDF_DLL} ${QPDF_IMPLIB}
						COMMAND ./configure --host=x86_64-w64-mingw32
						COMMAND make -j4
						WORKING_DIRECTORY ${qpdf_SOURCE_DIR}
						VERBATIM
						COMMENT "Configuring and building QPDF...")
	add_custom_target (qpdf_build DEPENDS ${QPDF_DLL} ${QPDF_IMPLIB})

	add_library (qpdf_lib SHARED IMPORTED)
	add_dependencies (qpdf_lib qpdf_build)
	set_target_properties (qpdf_lib PROPERTIES
						   IMPORTED_LOCATION ${QPDF_DLL}
						   IMPORTED_IMPLIB ${QPDF_IMPLIB})
	target_include_directories (qpdf_lib INTERFACE ${qpdf_SOURCE_DIR}/include/)

	install (FILES ${QPDF_DLL} DESTINATION bin)
endif ()

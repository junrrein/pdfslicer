include (Translations)
add_translations_directory (${GETTEXT_PACKAGE})
add_translations_catalog (${GETTEXT_PACKAGE}
	../src
	../src/backend
	../src/ui
	DESKTOP_FILES
	../data/pdfslicer.desktop.in
)

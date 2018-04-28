include (Translations)
add_translations_directory (${GETTEXT_PACKAGE})
add_translations_catalog (${GETTEXT_PACKAGE}
                          ../src
                          ../src/backend
                          ../src/ui
                          DESKTOP_FILES
						  ../data/${APPLICATION_ID}.desktop.in
						  APPDATA_FILES
						  ../data/${APPLICATION_ID}.appdata.xml.in)

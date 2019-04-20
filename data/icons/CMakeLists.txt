set (ICON_DIR_RELATIVE "share/icons/hicolor")

install (FILES
		${APPLICATION_ID}.svg
		${APPLICATION_ID}-symbolic.svg
		DESTINATION ${ICON_DIR_RELATIVE}/scalable/apps)

install (FILES
		DESTINATION ${ICON_DIR_RELATIVE}/scalable/actions)

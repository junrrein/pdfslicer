set (APPLICATION_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/application.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/backgroundthread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/settingsmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/aboutdialog.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/actionbar.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/appwindow.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/filechoosernative.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/headerbar.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/interactivepagewidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/openfiledialog.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/pagelabel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/pagewidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/previewwindow.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/savefiledialog.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/savingrevealer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/view.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/welcomescreen.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/zoomlevel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/zoomlevelwithactions.cpp)

target_sources (${CMAKE_PROJECT_NAME} PUBLIC ${APPLICATION_SOURCES})

set (SOURCES
	 ${CMAKE_CURRENT_SOURCE_DIR}/command.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/commandmanager.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/document.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/page.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/pdfsaver.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/pagerenderer.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/tempfile.cpp)

target_sources (${CMAKE_PROJECT_NAME} PUBLIC ${SOURCES})
target_include_directories (${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

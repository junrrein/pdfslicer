add_custom_target (uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake-modules/cmake_uninstall.cmake
)

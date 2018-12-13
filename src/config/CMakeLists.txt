configure_file (config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp)
target_sources (${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/config.cpp)
target_include_directories (${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library (CTPL INTERFACE)
target_sources (CTPL INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/ctpl_stl.h)
target_link_libraries (CTPL INTERFACE ${CMAKE_THREAD_LIBS_INIT})

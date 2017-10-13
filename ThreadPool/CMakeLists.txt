add_library (ThreadPool INTERFACE)
target_sources (ThreadPool INTERFACE
    ${CMAKE_SOURCE_DIR}/ThreadPool/ThreadPool.h)
target_link_libraries (ThreadPool INTERFACE ${CMAKE_THREAD_LIBS_INIT})

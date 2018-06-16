add_library (ConcurrentQueue INTERFACE)
target_sources (ConcurrentQueue INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/concurrentqueue.h)
target_link_libraries (ConcurrentQueue INTERFACE Threads::Threads)

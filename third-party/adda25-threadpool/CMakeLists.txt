add_library (ThreadPool INTERFACE)
target_sources (ThreadPool INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/threadpool.hpp)
target_link_libraries (ThreadPool INTERFACE Threads::Threads)

add_library (ThreadPool INTERFACE)
target_include_directories (ThreadPool SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (ThreadPool INTERFACE Threads::Threads)

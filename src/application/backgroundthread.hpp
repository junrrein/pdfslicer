#ifndef BACKGROUNDTHREAD_HPP
#define BACKGROUNDTHREAD_HPP

#include <threadpool.hpp>

namespace Slicer {

class BackgroundThread {
public:
    BackgroundThread();

    void pushBack(const std::function<void()>& task);
    void killRemainingTasks();

private:
    std::unique_ptr<astp::ThreadPool> m_threadpool;
};

} // namespace Slicer

#endif // BACKGROUNDTHREAD_HPP

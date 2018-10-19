/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                                                    *
*            _____ _                        _ ____             _                     *    
*           |_   _| |__  _ __ ___  __ _  __| |  _ \ ___   ___ | |                    *    
*             | | | '_ \| '__/ _ \/ _` |/ _` | |_) / _ \ / _ \| |                    *
*             | | | | | | | |  __/ (_| | (_| |  __/ (_) | (_) | |                    *
*             |_| |_| |_|_|  \___|\__,_|\__,_|_|   \___/ \___/|_|                    *
*                                                                                    *
*                   BECAUSE POWER IS NOTHING WITHOUT CONTROL                         *
*             You should not inherit from any of these classes:                      *
*                       no virtual destructors provided.                             *
*                                                                                    *
*                                                                                    *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **  
*                                                                                    *
*   The MIT License (MIT)                                                            *
*                                                                                    *
*   Copyright (c) 2016 Amedeo Setti                                                  *
*                                                                                    *
*   Permission is hereby granted, free of charge, to any person obtaining a copy     *
*   of this software and associated documentation files (the "Software"), to deal    *
*   in the Software without restriction, including without limitation the rights     *
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        *
*   copies of the Software, and to permit persons to whom the Software is            *
*   furnished to do so, subject to the following conditions:                         *
*                                                                                    *
*   The above copyright notice and this permission notice shall be included in all   *
*   copies or substantial portions of the Software.                                  *
*                                                                                    *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         *
*   FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE    *
*   SOFTWARE                                                                         *
*                                                                                    *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP
#ifdef __cplusplus
#include <future>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <deque>
#include <assert.h>
#include <exception>
#include <stdexcept>
#ifdef DEBUG
#include <iostream>
#endif

#ifndef TP_ENABLE_DEFAULT_EXCEPTION_CALL
#define TP_ENABLE_DEFAULT_EXCEPTION_CALL 1
#endif

#ifndef TP_ENABLE_SANITY_CHECKS
#define TP_ENABLE_SANITY_CHECKS 1
#endif

namespace astp {
/**
    *   Structure of the class:
    *
    *   private:
    *       - Nested Semaphore class
    *       - Nested DispatchGroup class
    *       - Nested ThreadsBlocker class
    *
    *   public:
    *       - API
    *
    *   private:
    *       - All the class variables and
    *         methods
    *   
    *   Enjoy!
    */
class ThreadPool {
private:
    /**
        *    ____                             _                    
        *   / ___|  ___ _ __ ___   __ _ _ __ | |__   ___  _ __ ___ 
        *   \___ \ / _ \ '_ ` _ \ / _` | '_ \| '_ \ / _ \| '__/ _ \
        *    ___) |  __/ | | | | | (_| | |_) | | | | (_) | | |  __/
        *   |____/ \___|_| |_| |_|\__,_| .__/|_| |_|\___/|_|  \___|
        *                              |_|                         
        *
        *
        *   Nested ThreadPool class 
        *   that represents a semaphore
        *   in order to make the ThreadPool thread safe.
        */
    class Semaphore {
    public:
        Semaphore(int value)
            : _value(value){};
        Semaphore(const Semaphore& S)
            : _mutex()
            , _cv(){};
        Semaphore& operator=(Semaphore S) { return *this; }
        ~Semaphore(){};

        void
        wait()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            --_value;
            if (_value < 0) {
                do {
                    _cv.wait(lock);
                } while (_wake_ups < 1);
                --_wake_ups;
            }
        }

        void
        signal()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ++_value;
            if (_value <= 0) {
                ++_wake_ups;
                _cv.notify_one();
            }
        }

    private:
        int _value;
        int _wake_ups = 0;
        std::mutex _mutex;
        std::condition_variable _cv;
    };

    /**
        *    ____  _                 _       _      ____                       
        *   |  _ \(_)___ _ __   __ _| |_ ___| |__  / ___|_ __ ___  _   _ _ __  
        *   | | | | / __| '_ \ / _` | __/ __| '_ \| |  _| '__/ _ \| | | | '_ \ 
        *   | |_| | \__ \ |_) | (_| | || (__| | | | |_| | | | (_) | |_| | |_) |
        *   |____/|_|___/ .__/ \__,_|\__\___|_| |_|\____|_|  \___/ \__,_| .__/ 
        *               |_|                                             |_|    
        *   
        *
        *   Nested ThreadPool class 
        *   that stores informations
        *   about dispatch groups.
        */
    class DispatchGroup {
    public:
        DispatchGroup(std::string id)
            : _id(id)
            , _closed(false)
            , _has_finished(false)
            , _jobs_done_counter(0)
            , _jobs_count_at_leave(0)
            , _sem_sync(Semaphore(1)){};
        DispatchGroup(DispatchGroup&& DP) noexcept
            : _id(DP.id())
            , _closed(DP.is_leave())
            , _has_finished(DP.has_finished())
            , _jobs_done_counter(0)
            , _jobs_count_at_leave(0)
            , _sem_sync(Semaphore(1)){};
        DispatchGroup& operator=(DispatchGroup&& DP) = default;
        DispatchGroup(const DispatchGroup& DP) = delete;
        DispatchGroup& operator=(const DispatchGroup& DP) = delete;

        ~DispatchGroup(){};

        void
        leave()
        {
            _closed = true;
            _jobs_count_at_leave = _jobs.size();
            _check_end_condition();
        }

        template <class T>
        void
        leave(T&& t)
        {
            leave();
            _end_action = t;
        }

        bool
        is_leave() const
        {
            return _closed;
        }

        template <class F>
        void
        insert(const F& f)
        {
            if (_closed)
                return;
            auto func = [=]() { f(); _signal_end_of_job(); };
            _jobs.push_back(func);
        }

        std::vector<std::function<void()>>
        jobs() { return _jobs; }

        bool
        has_finished() const
        {
            return _has_finished;
        }

        std::string
        id() const
        {
            return _id;
        }

        int
        jobs_count() const
        {
            return _jobs.size();
        }

        void
        synchronize()
        {
            _sem_sync.wait();
        }

        void
        end_synchronize()
        {
            _sem_sync.signal();
        }

    private:
        std::string _id;
        std::function<void()> _end_action;
        std::vector<std::function<void()>> _jobs;
        std::atomic<bool> _closed;
        std::atomic<bool> _has_finished;
        std::atomic<int> _jobs_done_counter;
        std::atomic<int> _jobs_count_at_leave;
        Semaphore _sem_sync;

        void
        _signal_end_of_job()
        {
            _jobs_done_counter++;
            _check_end_condition();
        }

        void
        _check_end_condition()
        {
            if (_jobs_done_counter == _jobs_count_at_leave && _closed) {
                _has_finished = true;
                if (_end_action)
                    _end_action();
            }
        }
    };

    /**
        *   Thread safe class that manage
        *   the waiting of the pool threads
        *   when the queue is empty.
        */
    class ThreadsBlocker {
    public:
        ThreadsBlocker()
            : _sem_interface(Semaphore(1)){};
        ~ThreadsBlocker(){};

        void
        activate_barrier()
        {
            _sem_interface.wait();
            _barrier = true;
            _sem_interface.signal();
        }

        void
        deactivate_barrier()
        {
            _sem_interface.wait();
            _barrier = false;
            _sem_interface.signal();
        }

        bool
        thread_wait(Semaphore* rsem)
        {
            _sem_interface.wait();
            if (_barrier) {
                _sem_interface.signal();
                return false;
            }
            _sems.push_back(rsem);
            _sem_interface.signal();
            return true;
        }

        void
        unblock(bool also_activate_barrier = false)
        {
            _sem_interface.wait();
            if (also_activate_barrier) {
                _barrier = true;
            }
            for (auto& s : _sems) {
                s->signal();
            }
            _sems.clear();
            _sem_interface.signal();
        }

    private:
        std::vector<Semaphore*> _sems;
        bool _barrier = false;
        Semaphore _sem_interface;
    };

    /**
        *       _    ____ ___ 
        *      / \  |  _ \_ _|
        *     / _ \ | |_) | | 
        *    / ___ \|  __/| | 
        *   /_/   \_\_|  |___|
        *
        */
public:
    /**
        *   If *max_threads* is not specified,
        *   the pool size is set to the max number
        *   of threads supported by the architecture.
        *   At least one thread is created.
        */
    ThreadPool(int max_threads = std::thread::hardware_concurrency()) noexcept(false)
        : _sem_api(Semaphore(1))
        , _sem_job_ins_container(Semaphore(1))
        , _thread_sleep_time_ns(1000)
        , _run_pool_thread(true)
        , _threads_count(0)
        , _thread_to_kill_c(0)
        , _push_c(0)
        , _prev_threads(0)
    {
#if TP_ENABLE_DEFAULT_EXCEPTION_CALL
        _exception_action = [](std::exception_ptr e) {};
#endif

#if TP_ENABLE_SANITY_CHECKS
        try {
            resize(max_threads);
        }
        catch (std::runtime_error e) {
            throw e;
        }
#else
        resize(max_threads);
#endif
    };

    /**
        *   Copy constructor.
        */
    ThreadPool(const ThreadPool& TP) = delete;

    /**
        *   Deleted assignment operators
        */
    ThreadPool& operator=(ThreadPool&& TP) = delete;
    ThreadPool& operator=(const ThreadPool& TP) = delete;

    /**
        *   When the ThreadPool is deallocated,
        *   the threads still running are joined().
        */
    ~ThreadPool() noexcept
    {
        try {
            if (_run_pool_thread) {
                _run_pool_thread = false;
                _threads_blocker.unblock(true);
                for (auto& t : _pool) {
                    t.join();
                }
            }
        }
        catch (...) {
        }
    };

    /**
        *   Update size for the thread pool;
        *   the abs value of num_threads is taken.
        */
    void
    resize(int num_threads = std::thread::hardware_concurrency()) noexcept(false)
    {
        if (!_run_pool_thread)
            return;

#if TP_ENABLE_SANITY_CHECKS
        _condition_check(errors.resize_alloc,
                         [&]() { return num_threads < 1; });
#endif

        _sem_api.wait();
        auto diff = abs(num_threads - _threads_count);
        if (num_threads > _threads_count) {
            for (auto i = 0; i < diff; ++i)
                _safe_thread_push();
        }
        else {
            for (auto i = 0; i < diff; ++i)
                _safe_thread_pop();
        }
        _threads_blocker.unblock();
        _sem_api.signal();
    }

    /**
        *   Push a job to do in jobs queue.
        *   Use lambda expressions in order to
        *   load jobs.
        */
    template <class F>
    ThreadPool&
    push(F&& f)
    {
        _safe_queue_push(f);
        return *this;
    }

    /**
        *   Push a job to do in jobs queue.
        *   Use lambda expressions in order to
        *   load jobs. Overload operator <<.
        */
    template <class F>
    ThreadPool&
    operator<<(F&& f)
    {
        _safe_queue_push(f);
        return *this;
    }

    /**
        *   Push multiple jobs to do in jobs queue.
        *   Use lambda expressions in order to
        *   load jobs.
        */
    template <class F, class... Args>
    ThreadPool&
    push(const F&& f, Args... args)
    {
        std::unique_lock<std::mutex> lock(_mutex_queue);
        _unsafe_queue_push(f);
        _unsafe_queue_push(args...);
        lock.unlock();
        return *this;
    }

    /**
        *   Insert and execute a task for a
        *   count number of times, and wait until
        *   execution is done.
        */
    template <class F>
    void
    apply_for(const int count, F&& f) noexcept(false)
    {
#if TP_ENABLE_SANITY_CHECKS
        _condition_check(errors.apply_it_num,
                         [&]() { return count < 0; });
#endif

        std::atomic<int> counter(0);
        auto func = [&]() { f(); ++counter; };

        std::unique_lock<std::mutex> lock(_mutex_queue);
        for (auto i = 0; i < count; ++i)
            _unsafe_queue_push_front(func);
        lock.unlock();

        while (counter != count) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(_thread_sleep_time_ns));
        }
    }

    template <class F>
    void
    apply_for_async(const int count, F&& f) noexcept(false)
    {
#if TP_ENABLE_SANITY_CHECKS
        _condition_check(errors.apply_it_num,
                         [&]() { return count < 0; });
#endif

        std::unique_lock<std::mutex> lock(_mutex_queue);
        for (auto i = 0; i < count; ++i)
            _unsafe_queue_push(f);
        lock.unlock();
    }

    /**
        *   Push a job in the queue and
        *   return a future, so you can 
        *   track and get the result of the lambda.
        *
        *   Inspired by vit-vit threadpool:
        *   https://github.com/vit-vit/CTPL
        */
    template <class F>
    auto
    future_from_push(F&& f) -> decltype(std::future<decltype(f())>())
    {
        auto packaged_task_ptr = std::make_shared<std::packaged_task<decltype(f())()>>(f);
        auto func = std::function<void()>([packaged_task_ptr]() { (*packaged_task_ptr)(); });
        _safe_queue_push(func);
        return packaged_task_ptr->get_future();
    }

    void
    synchronize()
    {
        _sem_job_ins_container.wait();
    }

    void
    end_synchronize()
    {
        _sem_job_ins_container.signal();
    }

    void
    awake()
    {
        if (_run_pool_thread)
            return;
        _run_pool_thread = true;
        resize(_prev_threads);
        _threads_blocker.deactivate_barrier();
    }

    /**
        *   Stop execution, detach all
        *   jobs under processing.
        *   This is a thread blocking call.
        */
    void
    stop()
    {
        if (!_run_pool_thread)
            return;
        _sem_api.wait();
        _run_pool_thread = false;
        _prev_threads = 0;

        _threads_blocker.unblock(true);

        while (_threads_count != 0) {
            ++_prev_threads;
            _safe_thread_pop();
        }
        while (_thread_to_kill_c != 0) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(_thread_sleep_time_ns));
        }
        _sem_api.signal();
    }

    /**
        *   Wait until all jobs
        *   are computed.
        *   This is a thread blocking call.
        */
    void
    wait()
    {
        if (!_run_pool_thread)
            return;
        while ((_push_c != 0)) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(_thread_sleep_time_ns));
        }
    }

    /**
        *   Returning the current size of the 
        *   thread pool.
        */
    int
    pool_size() const
    {
        return _threads_count;
    }

    size_t
    queue_size() const
    {
        return _push_c;
    }

    bool
    queue_is_empty() const
    {
        return _push_c == 0;
    }

    /**
        *   Set the thread sleep time.
        *   Interval is in nanoseconds.
        */
    void
    set_sleep_time_ns(const int time_ns) noexcept(false)
    {
#if TP_ENABLE_SANITY_CHECKS
        _condition_check(errors.sleep_time,
                         [&]() { return time_ns < 0; });
#endif
        _thread_sleep_time_ns = time_ns;
    }

    /**
        *   Set the thread sleep time.
        *   Interval is in milliseconds.
        */
    void
    set_sleep_time_ms(const int time_ms) noexcept(false)
    {
#if TP_ENABLE_SANITY_CHECKS
        _condition_check(errors.sleep_time,
                         [&]() { return time_ms < 0; });
#endif
        _thread_sleep_time_ns = time_ms * 1000000;
    }

    /**
        *   Set the thread sleep time.
        *   Interval is in seconds
        *   and can be a floating point value.
        */
    template <class F>
    void
    set_sleep_time_s(const F time_s) noexcept(false)
    {
#if TP_ENABLE_SANITY_CHECKS
        _condition_check(errors.sleep_time,
                         [&]() { return time_s < 0; });
#endif
        _thread_sleep_time_ns = static_cast<int>(time_s * 1000000000);
    }

    int
    sleep_time_ns() const
    {
        return _thread_sleep_time_ns;
    }

    /**
        *    ____   ____                           
        *   |  _ \ / ___|_ __ ___  _   _ _ __  ___ 
        *   | | | | |  _| '__/ _ \| | | | '_ \/ __|
        *   | |_| | |_| | | | (_) | |_| | |_) \__ \
        *   |____/ \____|_|  \___/ \__,_| .__/|___/
        *                               |_|        
        *
        *   Set of functions for command dispatch_group
        *   operations.
        *
        *
        *   Create a new group with an std::string 
        *   identifier.
        */
    void
    dg_open(const std::string& id) noexcept(false)
    {
        std::unique_lock<std::mutex> lock(_mutex_groups);
        std::map<std::string, DispatchGroup>::iterator it;
        if (_unsafe_dg_id_check(id, it)) {
#if TP_ENABLE_SANITY_CHECKS
            throw std::runtime_error(errors.dg_not_empty(id));
#else
            return;
#endif
        }
        _groups.insert(std::make_pair(id, DispatchGroup(id)));
    }

    /**
        *   Insert a job to do in a specific group.
        *   If the group not exist, nothing is done.
        *   Task will not start until a call to 
        *   leave will be done.
        */
    template <class F>
    void
    dg_insert(const std::string& id, F&& f) noexcept(false)
    {
        std::unique_lock<std::mutex> lock(_mutex_groups);
        std::map<std::string, DispatchGroup>::iterator it;
        if (!_unsafe_dg_id_check(id, it)) {
#if TP_ENABLE_SANITY_CHECKS
            throw std::runtime_error(errors.dg_empty(id));
#else
            return;
#endif
        }
        it->second.insert(f);
    }

    /**
        *   Create a new group, insert a job on it
        *   and dispatch it.
        *   It is guaranteed that this job will be
        *   the first next job to be processed by
        *   the threadpool.
        */
    template <class F>
    void
    dg_now(const std::string& id, F&& f) noexcept(false)
    {
        std::unique_lock<std::mutex> lock(_mutex_groups);
        std::map<std::string, DispatchGroup>::iterator it;
        if (_unsafe_dg_id_check(id, it)) {
#if TP_ENABLE_SANITY_CHECKS
            throw std::runtime_error(errors.dg_not_empty(id));
#else
            return;
#endif
        }
        _groups.insert(std::make_pair(id, DispatchGroup(id)));
        it = _groups.find(id);
        it->second.insert(f);
        it->second.leave();
        _safe_queue_push_front(it->second.jobs()[0]);
    }

    /**
        *   Signal to a group that the jobs immission 
        *   is end, than start pushing the group jobs
        *   to the standard threadpool queue. At the end
        *   of the tasks, it will execute the f action,
        *   like a barrier.
        */
    template <class F>
    void
    dg_close_with_barrier(const std::string& id, const F&& f) noexcept(false)
    {
        std::unique_lock<std::mutex> lock(_mutex_groups);
        std::map<std::string, DispatchGroup>::iterator it;
        if (!_unsafe_dg_id_check(id, it)) {
#if TP_ENABLE_SANITY_CHECKS
            throw std::runtime_error(errors.dg_empty(id));
#else
            return;
#endif
        }
        it->second.leave(f);
        auto jobs = it->second.jobs();
        for (auto& j : jobs) {
            push(j);
        }
    }

    /**
        *   Signal to a group that the jobs immission 
        *   is end, than start pushing the group jobs
        *   to the standard threadpool queue.
        */
    void
    dg_close(const std::string& id) noexcept(false)
    {
        std::unique_lock<std::mutex> lock(_mutex_groups);
        std::map<std::string, DispatchGroup>::iterator it;
        if (!_unsafe_dg_id_check(id, it)) {
#if TP_ENABLE_SANITY_CHECKS
            throw std::runtime_error(errors.dg_empty(id));
#else
            return;
#endif
        }
        it->second.leave();
        auto jobs = it->second.jobs();
        for (auto& j : jobs) {
            push(j);
        }
    }

    /**
        *   Wait until every job in a group is computed.
        *   This is a thread blocking call.
        */
    void
    dg_wait(const std::string& id) noexcept(false)
    {
        std::map<std::string, DispatchGroup>::iterator it;
        if (!_unsafe_dg_id_check(id, it)) {
#if TP_ENABLE_SANITY_CHECKS
            throw std::runtime_error(errors.dg_empty(id));
#else
            return;
#endif
        }
        while (!it->second.has_finished()) {
            std::chrono::nanoseconds(0);
        }
        _groups.erase(it);
    }

    /**
        *   Wait until every job in a group is computed.
        *   This is a thread blocking call. 
        *   At the end execute the callback;
        */
    template <class F>
    void
    dg_wait(const std::string& id, F&& f) noexcept(false)
    {
#if TP_ENABLE_SANITY_CHECKS
        try {
            dg_wait(id);
            f();
        }
        catch (std::runtime_error e) {
            throw e;
        }
        catch (...) {
            throw;
        }
#else
        dg_wait(id);
        try {
            f();
        }
        catch (...) {
            throw;
        }
#endif
    }

    /**
        *   The same as synchronize, but is useful
        *   if you don't want do block all others
        *   jobs in the queue.
        */
    void
    dg_synchronize(const std::string& id) noexcept(false)
    {
        std::unique_lock<std::mutex> lock(_mutex_groups);
        std::map<std::string, DispatchGroup>::iterator it;
        if (!_unsafe_dg_id_check(id, it)) {
#if TP_ENABLE_SANITY_CHECKS
            throw std::runtime_error(errors.dg_empty(id));
#else
            return;
#endif
        }
        it->second.synchronize();
    }
    /**/
    void
    dg_end_synchronize(const std::string id) noexcept(false)
    {
        std::unique_lock<std::mutex> lock(_mutex_groups);
        std::map<std::string, DispatchGroup>::iterator it;
        if (!_unsafe_dg_id_check(id, it)) {
#if TP_ENABLE_SANITY_CHECKS
            throw std::runtime_error(errors.dg_empty(id));
#else
            return;
#endif
        }
        it->second.end_synchronize();
    }

    /**
        *   Set a callback for excpetion handling.
        *   If not setted, threadpool has a default
        *   callback, that does nothing and not
        *   rethrow.
        */
    template <class F>
    void
    set_excpetion_action(std::function<void(F)> f)
    {
        auto func = [&f](std::exception_ptr excp) {
            try {
                std::rethrow_exception(excp);
            }
            catch (F e) {
                f(e);
            }
        };
        _sem_api.wait();
        _exception_action = func;
        _sem_api.signal();
    }

    /**
        *    ____       _            _       
        *   |  _ \ _ __(_)_   ____ _| |_ ___ 
        *   | |_) | '__| \ \ / / _` | __/ _ \
        *   |  __/| |  | |\ V / (_| | ||  __/
        *   |_|   |_|  |_| \_/ \__,_|\__\___|
        *
        */
private:
    /**
        *   Mutex for queue access. 
        */
    std::mutex _mutex_queue;
    /**
        *   Mutex for pool resize. 
        */
    std::mutex _mutex_pool;
    /**
        *   Mutex for groups access. 
        */
    std::mutex _mutex_groups;
    /**
        *   Semaphore for class thread-safety. 
        */
    Semaphore _sem_api;
    /**
        *   Optional semaphore for jobs lambda data
        *   protection in critical sections.
        */
    Semaphore _sem_job_ins_container;
    /**
        *   Time in nanoseconds which threads
        *   that are sleeping check for new
        *   jobs in the queue.
        */
    std::atomic<int> _thread_sleep_time_ns;
    /**
        *   Flag for pool's threads state,
        *   when false, all the threads will be
        *   detached.
        */
    std::atomic<bool> _run_pool_thread;
    /**
        *   Where the running threads lives. 
        */
    std::vector<std::thread> _pool;
    /**
        *   Queue of jobs to do.
        */
    std::deque<std::function<void()>> _queue;
    /**
        *   A map of in process groups of jobs.
        */
    std::map<std::string, DispatchGroup> _groups;
    /**
        *   The number of threads currently in the pool.
        */
    std::atomic<int> _threads_count;
    /**
        *   Counter used when there are 
        *   some threads to remove from
        *   the pool [stop or resize]. 
        */
    std::atomic<int> _thread_to_kill_c;
    /**
        *   Stores the id's of the threads
        *   that will be kills.
        */
    std::vector<std::thread::id> _threads_to_kill_id;
    /**
        *   When zero means that all the task
        *   were executed and no one is 
        *   waiting.
        */
    std::atomic<int> _push_c;
    /**
        *   Number of threads that the pool had
        *   when a stop() was called. Used
        *   by the awake() method to restore the 
        *   same number of threads.
        */
    std::atomic<int> _prev_threads;
    /**
        *   Callback for excpetion handling setted by the user.
        */
    std::function<void(std::exception_ptr)> _exception_action;
    std::mutex _mutex_exceptions;
    /**
        *   Manage the threads waiting.
        */
    ThreadsBlocker _threads_blocker;
    /**
        *   For speedup.
        */
    bool _queue_empty = true;

    /**
        *   String errors that are throw when user 
        *   submit wrong inputs or try to do illegal
        *   operations.
        */
    struct Errors {
        std::string
        dg_empty(const std::string& id)
        {
            return "ThreadPool: group with id " + id + " not exist";
        };

        std::string
        dg_not_empty(const std::string& id)
        {
            return "ThreadPool: group with id " + id + " already exist";
        };

        std::string sleep_time = "ThreadPool: sleep time value must be greater or equal to zero";

        std::string apply_it_num = "ThreadPool: Number of iterations in apply must be greater than zero";

        std::string resize_alloc = "ThreadPool: Number of threads in resize or alloc must be greater than zero";
    } errors;

    /**
        *   Given a condition to check, throw an error
        *   if the condition is true.
        */
    template <class M, class T>
    void
    _condition_check(M&& m, T&& t) noexcept(false)
    {
        if (t())
            throw std::runtime_error(m);
    }

    /**
        *   Check if the groups map contains or
        *   not the required id. Used
        *   by dispatch_group methods.
        */
    bool
    _unsafe_dg_id_check(const std::string& id,
                        std::map<std::string, DispatchGroup>::iterator& it)
    {
        it = _groups.find(id);
        return (it == _groups.end()) ? false : true;
    }

    /**
        *   Called by pools threads when
        *   an excpetion occours.
        */
    template <class F>
    void
    _exc_exception_action(F excpetion)
    {
        _exception_action(excpetion);
    }

    /**
        *   Lock the queue mutex for
        *   a safe insertion in the queue.
        */
    template <class F>
    void
    _safe_queue_push(F&& t)
    {
        ++_push_c;
        std::unique_lock<std::mutex> lock(_mutex_queue);
        _queue.push_back(std::move(t));
        if (_queue_empty)
            _threads_blocker.unblock();
    }

    /**
        *   Modify the queue in UNSAFE 
        *   manner, so you should lock
        *   the queue outside this function.
        */
    template <class F>
    void
    _unsafe_queue_push(F&& t)
    {
        ++_push_c;
        _queue.push_back(std::move(t));
        if (_queue_empty)
            _threads_blocker.unblock();
    }

    /**
        *   Modify the queue in UNSAFE 
        *   manner, so you should lock
        *   the queue outside this function.
        *   Used for push with variadic templates.
        */
    template <class F, class... Args>
    void
    _unsafe_queue_push(F&& t, Args... args)
    {
        ++_push_c;
        _queue.push_back(std::move(t));
        _unsafe_queue_push(args...);
        if (_queue_empty)
            _threads_blocker.unblock();
    }

    /**
        *   Lock the queue mutex for
        *   a safe insertion in the queue.
        *   Insert the element at end of the 
        *   queue.
        */
    template <class F>
    void
    _safe_queue_push_front(F&& t)
    {
        ++_push_c;
        std::unique_lock<std::mutex> lock(_mutex_queue);
        _queue.push_front(std::move(t));
        if (_queue_empty)
            _threads_blocker.unblock();
    }

    /**
        *   Modify the queue in UNSAFE 
        *   manner, so you should lock
        *   the queue outside this function.
        *   Insert the element at end of the 
        *   queue.
        */
    template <class F>
    void
    _unsafe_queue_push_front(F&& t)
    {
        ++_push_c;
        _queue.push_front(std::move(t));
        if (_queue_empty)
            _threads_blocker.unblock();
    }

    /**
        *   Lock the queue mutex, safely pop
        *   job from the queue if not empty.
        */
    std::function<void()>
    _safe_queue_pop()
    {
        std::unique_lock<std::mutex> lock(_mutex_queue);
        if (_queue.empty()) {
            _queue_empty = true;
            return std::function<void()>();
        }

        auto t = _queue.front();
        _queue.pop_front();
        _queue_empty = false;
        return t;
    }

    /**
        *   Called when the ThreadPool is created 
        *   or the user has required a resize 
        *   operation.
        */
    void
    _safe_thread_push()
    {
        std::unique_lock<std::mutex> lock(_mutex_pool);
        _pool.push_back(std::thread(&ThreadPool::_thread_loop_mth, this));
        ++_threads_count;
    }

    /**
        *   Called when the ThreadPool is deleted 
        *   or the user has required both a resize 
        *   operation or a stop operation.
        */
    void
    _safe_thread_pop()
    {
        std::unique_lock<std::mutex> lock(_mutex_pool);
        if (_pool.empty())
            return;

        ++_thread_to_kill_c;
        _threads_to_kill_id.push_back(_pool.back().get_id());
        _pool.back().detach();
        _pool.pop_back();
        --_threads_count;
    }

    /**
        *   Called by each thread in the pool
        *   when _thread_to_kill_c != 0.
        *   Than the thread will know if must 
        *   exit from the loop.
        */
    bool
    _thread_is_to_kill(std::thread::id id)
    {
        std::unique_lock<std::mutex> lock(_mutex_pool);
        std::vector<std::thread::id>::iterator it = std::find_if(_threads_to_kill_id.begin(),
                                                                 _threads_to_kill_id.end(),
                                                                 [id](std::thread::id tid) { return (tid == id); });
        return (it == _threads_to_kill_id.end()) ? false : true;
    }

    /**
        *   Each thread start run this function
        *   when the thread is created, and 
        *   exit only when the pool is destructed
        *   or the stop() function is called.
        *   The thread go to sleep if the 
        *   queue is empty. 
        */
    void
    _thread_loop_mth()
    {
        Semaphore sem(0);
        while (_run_pool_thread) {
            if (_thread_to_kill_c != 0) {
                if (_thread_is_to_kill(std::this_thread::get_id()))
                    break;
            }
            auto funcf = _safe_queue_pop();
            if (!funcf) {
                if (_threads_blocker.thread_wait(&sem))
                    sem.wait();
                continue;
            }
            try {
                funcf();
            }
            catch (...) {
                std::unique_lock<std::mutex> lock(_mutex_exceptions);
                _exc_exception_action(std::current_exception());
            }
            --_push_c;
        }
        --_thread_to_kill_c;
    }

}; /* End ThreadPool */

}; /* Namespace end */

#endif /* __cplusplus */

#endif /* THREAD_POOL_HPP */

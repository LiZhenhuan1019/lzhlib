#ifndef LZHLIB_THREAD_THREAD_POOL_HPP
#define LZHLIB_THREAD_THREAD_POOL_HPP

#include <thread>                   // for std::thread
#include <vector>                   // for std::vector
#include <cstddef>                  // for std::size_t
#include <atomic>
#include <future>                   // for std::future, std::packaged_task
#include <type_traits>              // for std::invoke_result_t
#include <mutex>                    // for std::mutex
#include <stdexcept>                // for std::runtime_error
#include <lzhlib/thread/queue.hpp>  // for lzhlib::thread::queue
namespace lzhlib
{
    namespace thread
    {
        struct thread_joiner
        {
            explicit thread_joiner(std::thread &thread)
                : thread(thread)
            {}
            ~thread_joiner()
            {
                thread.join();
            }
        private:
            std::thread &thread;
        };
        struct threads_joiner
        {
            explicit threads_joiner(std::vector<std::thread> &threads)
                : threads(threads)
            {}
            ~threads_joiner()
            {
                for (auto &thread : threads)
                    thread.join();
            }
        private:
            std::vector<std::thread> &threads;
        };
        // Thrown by thread_pool::submit() once the pool has been stopped.
        struct thread_pool_stopped : public std::runtime_error
        {
            thread_pool_stopped()
                : std::runtime_error("in lzhlib::thread::thread_pool: the pool is stopped.")
            {}
        };
        template <typename Prototype = void(void)>
        class thread_pool
        {
            using prototype = Prototype;
            using task_type = std::packaged_task<prototype>;
            using task_queue = queue<task_type>;
        public:
            explicit thread_pool(std::size_t thread_number = std::thread::hardware_concurrency())
            {
                try
                {
                    workers.reserve(thread_number);
                    for (std::size_t i = 0; i < thread_number; ++i)
                        workers.emplace_back(&thread_pool::worker_function, this);
                }
                catch (...)
                {
                    // Finish the threads that were already started, then let the
                    // exception escape: a half constructed pool would otherwise leak
                    // joinable threads.
                    stop();
                    throw;
                }
            }
            thread_pool(thread_pool const &) = delete;
            thread_pool &operator=(thread_pool const &) = delete;
            ~thread_pool()
            {
                stop();
            }

            // Stops accepting new tasks, runs everything that is still queued and
            // joins the workers; idempotent, so the destructor can call it again.
            // A concurrent caller blocks until the first one has finished joining,
            // and it must not be called from a task running in this pool (a thread
            // cannot join itself).
            void stop()
            {
                std::lock_guard<std::mutex> lock(stop_mutex);
                if (stopped.load())
                    return;
                stopped.store(true);
                tasks.close();  // wakes the workers, the queued tasks are still drained
                for (std::thread &worker : workers)
                {
                    if (worker.joinable())
                        worker.join();
                }
            }
            bool is_stopped() const
            {
                return stopped.load();
            }

            template <typename FunctionType>
            std::future<std::invoke_result_t<prototype>> submit(FunctionType f)
            {
                if (stopped.load())
                    throw thread_pool_stopped();
                task_type task(std::move(f));
                auto future = task.get_future();
                try
                {
                    tasks.push(std::move(task));
                }
                catch (typename task_queue::queue_closed const &)
                {
                    // stop() closed the queue between the check above and the push.
                    throw thread_pool_stopped();
                }
                return future;
            }
        private:
            std::atomic<bool> stopped{false};
            std::mutex stop_mutex;
            task_queue tasks;
            std::vector<std::thread> workers;

            void worker_function()
            {
                task_type task;
                // wait_and_pop returns false as soon as the queue is closed and empty,
                // so close() makes the workers drain the backlog and then exit.
                while (tasks.wait_and_pop(task))
                {
                    try
                    {
                        task();
                    }
                    catch (...)
                    {
                        // packaged_task stores the exception of the task in its future;
                        // this only swallows an exception from an empty task object.
                    }
                }
            }
        };
    }
}

#endif //LZHLIB_THREAD_THREAD_POOL_HPP

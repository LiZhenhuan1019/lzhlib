#ifndef LZHLIB_THREAD_THREAD_POOL_HPP
#define LZHLIB_THREAD_THREAD_POOL_HPP

#include <thread>                   // for std::thread
#include <vector>                   // for std::vector
#include <cstddef>                  // for std::size_t
#include <atomic>
#include <future>                   // for std::future, std::packaged_task
#include <type_traits>              // for std::invoke_result_t
#include <chrono>
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
        template <typename Prototype = void(void)>
        class thread_pool
        {
            using prototype = Prototype;
        public:
            explicit thread_pool(std::size_t thread_number = std::thread::hardware_concurrency())
                : done(false), joiner(workers)
            {
                try
                {
                    for (std::size_t i = 0; i < thread_number; ++i)
                        workers.emplace_back(&thread_pool::worker_function, this);
                }
                catch (...)
                {
                    done = true;
                    throw;
                }
            }
            ~thread_pool()
            {
                done = true;
            }
            template <typename FunctionType>
            std::future<std::invoke_result_t<prototype>> submit(FunctionType f)
            {
                std::packaged_task<prototype> task(std::move(f));
                auto future = task.get_future();
                tasks.push(std::move(task));
                return future;
            }
        private:
            std::atomic_bool done;
            queue <std::packaged_task<prototype>> tasks;
            std::vector<std::thread> workers;
            threads_joiner joiner;

            void worker_function()
            {
                while (!done)
                {
                    using namespace std::chrono_literals;
                    if (auto task = tasks.wait_for_and_pop(500ms))
                    {
                        try
                        {
                            task.value()();
                        }
                        catch (...)
                        {}
                    }
                }
            }
        };
    }
}

#endif //LZHLIB_THREAD_THREAD_POOL_HPP

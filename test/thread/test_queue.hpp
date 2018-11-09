#ifndef LZHLIB_TEST_QUEUE_HPP
#define LZHLIB_TEST_QUEUE_HPP
#include <iostream>
#include <string>
#include <atomic>
#include <thread>
#include <lzhlib/thread/queue.hpp>
namespace lzhlib::test
{
    namespace thread
    {
        namespace queue
        {
            namespace detail
            {
                std::atomic_flag begin;
                template <typename Queue>
                void writer(Queue &queue, int number)
                {
                    std::string s = "hello";
                    for (int i = 0; i < 10; ++i)
                    {
                        using namespace std::literals;
                        queue.push(std::to_string(i));
                        std::cout << "Write thread "s.append(std::to_string(number)).append(" push ").append(std::to_string(i)).append("\n");
                    }
                }
                template <typename Queue>
                void reader(Queue &queue, int number)
                {
                    for (int i = 0; i < 10; ++i)
                    {
                        using namespace std::literals;
                        auto s = queue.wait_and_pop();
                        std::cout << "Read thread "s.append(std::to_string(number)).append(" pop ").append(s).append("\n");
                    }
                }
            }
            void test()
            {
                using queue_t = lzhlib::thread::queue<std::string>;
                queue_t q;
                std::thread w0(detail::writer<queue_t>, std::ref(q), 0);
                std::thread w1(detail::writer<queue_t>, std::ref(q), 1);
                std::thread w2(detail::writer<queue_t>, std::ref(q), 2);
                std::thread r0(detail::reader<queue_t>, std::ref(q), 0);
                std::thread r1(detail::reader<queue_t>, std::ref(q), 1);
                std::thread r2(detail::reader<queue_t>, std::ref(q), 2);
                w0.join();
                w1.join();
                w2.join();
                r0.join();
                r1.join();
                r2.join();
            }
        }
    }
}
#endif //LZHLIB_TEST_QUEUE_HPP

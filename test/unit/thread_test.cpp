#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <future>
#include <numeric>
#include <thread>
#include <vector>

#include "lzhlib/thread/queue.hpp"
#include "lzhlib/thread/thread_pool.hpp"

namespace
{
    using namespace std::chrono_literals;
    using lzhlib::thread::queue;
    using lzhlib::thread::thread_pool;
}

TEST(queue, try_pop_on_empty_queue_returns_nullopt)
{
    queue<int> q;
    EXPECT_FALSE(q.try_pop().has_value());
}

TEST(queue, push_then_try_pop)
{
    queue<int> q;
    q.push(42);

    auto value = q.try_pop();
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(*value, 42);
    EXPECT_FALSE(q.try_pop().has_value());
}

TEST(queue, wait_for_and_pop_times_out_on_empty_queue)
{
    queue<int> q;
    auto start = std::chrono::steady_clock::now();
    auto value = q.wait_for_and_pop(20ms);

    EXPECT_FALSE(value.has_value());
    EXPECT_GE(std::chrono::steady_clock::now() - start, 20ms);
}

TEST(queue, wait_for_and_pop_returns_value)
{
    queue<int> q;
    q.push(7);
    auto value = q.wait_for_and_pop(1s);

    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(*value, 7);
}

TEST(queue, producer_consumer_transfers_every_element)
{
    constexpr int count = 2000;

    queue<int> q;
    std::thread producer{[&q] {
        for (int i = 0; i < count; ++i)
        {
            q.push(i);
        }
    }};

    long long sum = 0;
    for (int i = 0; i < count; ++i)
    {
        sum += q.wait_and_pop();
    }
    producer.join();

    EXPECT_EQ(sum, static_cast<long long>(count) * (count - 1) / 2);
}

TEST(thread_pool, submit_runs_every_task)
{
    std::atomic<int> executed{0};
    std::vector<std::future<void>> futures;

    {
        thread_pool<> pool(4);
        for (int i = 0; i < 200; ++i)
        {
            futures.push_back(pool.submit([&executed] { ++executed; }));
        }
        for (auto &future : futures)
        {
            future.get();
        }
        EXPECT_EQ(executed.load(), 200);
    }

    EXPECT_EQ(executed.load(), 200);
}

TEST(thread_pool, tasks_are_executed_concurrently)
{
    std::atomic<int> running{0};
    std::atomic<int> max_running{0};
    std::vector<std::future<void>> futures;

    {
        thread_pool<> pool(4);
        for (int i = 0; i < 16; ++i)
        {
            futures.push_back(pool.submit([&running, &max_running] {
                int current = ++running;
                int observed = max_running.load();
                while (observed < current && !max_running.compare_exchange_weak(observed, current))
                {
                }
                std::this_thread::sleep_for(5ms);
                --running;
            }));
        }
        for (auto &future : futures)
        {
            future.get();
        }
    }

    EXPECT_GE(max_running.load(), 2);
}

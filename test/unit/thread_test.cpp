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

TEST(queue, close_wakes_a_consumer_that_is_already_blocked)
{
    queue<int> q;
    std::atomic<bool> waiting{false};
    bool popped = true;

    std::thread consumer{[&q, &waiting, &popped] {
        int value = 0;
        waiting.store(true);
        popped = q.wait_and_pop(value);
    }};
    while (!waiting.load())
    {
        std::this_thread::yield();
    }
    std::this_thread::sleep_for(20ms);
    q.close();
    consumer.join();

    EXPECT_FALSE(popped);
    EXPECT_TRUE(q.is_closed());
}

TEST(queue, close_lets_a_consumer_drain_the_elements_that_are_already_queued)
{
    queue<int> q;
    q.push(1);
    q.push(2);
    q.close();

    int value = 0;
    EXPECT_TRUE(q.wait_and_pop(value));
    EXPECT_EQ(value, 1);
    EXPECT_TRUE(q.wait_and_pop(value));
    EXPECT_EQ(value, 2);
    EXPECT_FALSE(q.wait_and_pop(value));
}

TEST(queue, push_on_a_closed_queue_throws)
{
    queue<int> q;
    q.close();

    EXPECT_THROW(q.push(1), queue<int>::queue_closed);
    EXPECT_FALSE(q.try_pop().has_value());
}

TEST(queue, value_returning_wait_and_pop_throws_when_the_closed_queue_is_drained)
{
    queue<int> q;
    q.close();

    EXPECT_THROW(q.wait_and_pop(), queue<int>::queue_closed);
}

TEST(thread_pool, destructor_runs_the_tasks_that_are_still_queued)
{
    std::atomic<int> executed{0};
    std::vector<std::future<void>> futures;

    {
        thread_pool<> pool(2);
        for (int i = 0; i < 100; ++i)
        {
            futures.push_back(pool.submit([&executed] {
                std::this_thread::sleep_for(1ms);
                ++executed;
            }));
        }
    }  // the destructor has to drain the queue before it joins the workers

    EXPECT_EQ(executed.load(), 100);
    for (std::future<void> &future : futures)
    {
        EXPECT_NO_THROW(future.get());
    }
}

TEST(thread_pool, stop_runs_queued_tasks_and_rejects_new_submissions)
{
    thread_pool<> pool(2);
    std::atomic<int> executed{0};
    EXPECT_FALSE(pool.is_stopped());

    auto future = pool.submit([&executed] {
        std::this_thread::sleep_for(10ms);
        ++executed;
    });
    pool.stop();

    EXPECT_TRUE(pool.is_stopped());
    EXPECT_NO_THROW(future.get());
    EXPECT_EQ(executed.load(), 1);
    EXPECT_THROW(pool.submit([] {}), lzhlib::thread::thread_pool_stopped);
    EXPECT_NO_THROW(pool.stop());  // idempotent, the destructor calls it again
}

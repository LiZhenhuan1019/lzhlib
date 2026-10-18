#ifndef LZHLIB_THREAD_QUEUE_HPP
#define LZHLIB_THREAD_QUEUE_HPP
#include <type_traits>
#include <optional>
#include <chrono>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <stdexcept>
namespace lzhlib
{
    namespace thread
    {
        template <typename T>
        struct queue
        {
            struct node
            {
                std::optional<T> data;
                std::unique_ptr<node> next;
            };

        public:
            // Thrown by push() and by the value returning wait_and_pop() once the
            // queue is closed, so that a shut down producer / consumer cannot hang.
            struct queue_closed : public std::runtime_error
            {
                queue_closed()
                    : std::runtime_error("in lzhlib::thread::queue: the queue is closed.")
                {}
            };
            queue()
                : head(std::make_unique<node>()), tail(head.get())
            {}
            queue(queue const &) = delete;
            queue const &operator=(queue const &) = delete;

            template <typename Duration, typename = std::enable_if_t<std::is_nothrow_move_constructible_v<T>>>
            std::optional<T> wait_for_and_pop(Duration duration)
            {
                std::unique_ptr<node> popped = wait_pop_head_for(duration);
                return popped ? std::move(popped->data) : std::nullopt;
            }
            template <typename Duration, typename = std::enable_if_t<std::is_nothrow_move_assignable_v<T>>>
            bool wait_for_and_pop(T &out_value, Duration duration)
            {
                std::unique_ptr<node> popped = wait_pop_head_for(duration);
                if (popped)
                {
                    out_value = std::move(popped->data.value());
                    return true;
                }
                return false;
            }
            // Blocks until an element is available. Throws queue_closed() when the
            // queue has been closed and every queued element has been popped, so a
            // consumer that loops on this call still terminates after close().
            template <typename = std::enable_if_t<std::is_nothrow_move_constructible_v<T>>>
            T wait_and_pop()
            {
                std::unique_ptr<node> popped = wait_pop_head();
                if (!popped)
                    throw queue_closed();
                return std::move(popped->data.value());
            }
            // Blocks until an element is available; returns false when the queue has
            // been closed and drained.
            template <typename = std::enable_if_t<std::is_nothrow_move_assignable_v<T>>>
            bool wait_and_pop(T &out_value)
            {
                std::unique_ptr<node> popped = wait_pop_head();
                if (!popped)
                    return false;
                out_value = std::move(popped->data.value());
                return true;
            }
            template <typename = std::enable_if_t<std::is_nothrow_move_constructible_v<T>>>
            std::optional<T> try_pop()
            {
                std::unique_ptr<node> popped = try_pop_head();
                return popped ? std::move(popped->data) : std::nullopt;
            }
            template <typename = std::enable_if_t<std::is_nothrow_move_assignable_v<T>>>
            bool try_pop(T &out_value)
            {
                std::unique_ptr<node> popped = try_pop_head();
                if (popped)
                {
                    out_value = std::move(popped->data.value());
                    return true;
                }
                return false;
            }
            // Throws queue_closed() once close() has been called.
            template <typename = std::enable_if_t<std::is_nothrow_move_constructible_v<T>>>
            void push(T value)
            {
                if (is_closed())
                    throw queue_closed();
                std::unique_ptr<node> new_node = std::make_unique<node>();
                node *const new_tail = new_node.get();
                {
                    std::lock_guard<std::mutex> tail_lock(tail_mutex);
                    if (closed.load())
                        throw queue_closed();
                    tail->data.emplace(std::move(value));
                    tail->next = std::move(new_node);
                    tail = new_tail;
                }
                has_new_element.notify_one();
            }

            // Wakes every blocked consumer. Elements that are already queued can still
            // be popped, but push() throws from now on. A consumer that loops
            // `while (queue.wait_and_pop(value)) ...` therefore drains the remaining
            // elements and then finishes instead of blocking forever.
            void close()
            {
                closed.store(true);
                has_new_element.notify_all();
            }
            bool is_closed() const
            {
                return closed.load();
            }
        private:
            std::mutex head_mutex;
            std::unique_ptr<node> head;
            std::mutex tail_mutex;
            node *tail;
            std::condition_variable has_new_element;
            std::atomic<bool> closed{false};

            node *get_tail()
            {
                std::lock_guard<std::mutex> tail_lock(tail_mutex);
                return tail;
            }
            bool is_empty()
            {
                return head.get() == get_tail();
            }
            std::unique_ptr<node> pop_head()
            {
                std::unique_ptr<node> popped_head = std::move(head);
                head = std::move(popped_head->next);
                return popped_head;
            }
            template <typename Duration>
            std::unique_ptr<node> wait_pop_head_for(Duration duration)
            {
                std::unique_lock<std::mutex> head_lock(head_mutex);
                // wait_for on a steady clock: a wall clock jump must not stretch the
                // timeout (wait_until on system_clock was used here before).
                if (!has_new_element.wait_for(head_lock, duration, [this] { return closed.load() || !is_empty(); }))
                    return nullptr;  // timed out
                if (is_empty())
                    return nullptr;  // closed and drained
                return pop_head();
            }
            std::unique_ptr<node> wait_pop_head()
            {
                std::unique_lock<std::mutex> head_lock(head_mutex);
                has_new_element.wait(head_lock, [this] { return closed.load() || !is_empty(); });
                if (is_empty())
                    return nullptr;  // closed and drained
                return pop_head();
            }
            std::unique_ptr<node> try_pop_head()
            {
                std::lock_guard<std::mutex> head_lock(head_mutex);
                if (is_empty())
                    return nullptr;
                return pop_head();
            }
        };
    }
}
#endif //LZHLIB_THREAD_QUEUE_HPP

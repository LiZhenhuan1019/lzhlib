#ifndef LZHLIB_QUEUE_HPP
#define LZHLIB_QUEUE_HPP
#include <type_traits>
#include <optional>
#include <chrono>
#include <memory>
#include <mutex>
#include <condition_variable>
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
            template <typename = std::enable_if_t<std::is_nothrow_move_constructible_v<T>>>
            T wait_and_pop()
            {
                return std::move(wait_pop_head()->data.value());
            }
            template <typename = std::enable_if_t<std::is_nothrow_move_assignable_v<T>>>
            void wait_and_pop(T &out_value)
            {
                out_value = std::move(wait_pop_head()->data.value());
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
                return popped;
            }
            void push(T value)
            {
                std::unique_ptr<node> new_node = std::make_unique<node>();
                node *const new_tail = new_node.get();
                {
                    std::lock_guard<std::mutex> tail_lock(tail_mutex);
                    tail->data = std::move(value);
                    tail->next = std::move(new_node);
                    tail = new_tail;
                }
                has_new_element.notify_one();
            }
        private:
            std::mutex head_mutex;
            std::unique_ptr<node> head;
            std::mutex tail_mutex;
            node *tail;
            std::condition_variable has_new_element;

            node *get_tail()
            {
                std::lock_guard<std::mutex> tail_lock(tail_mutex);
                return tail;
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
                auto now = std::chrono::system_clock::now();
                std::unique_lock<std::mutex> head_lock(head_mutex);
                using namespace std::chrono_literals;
                if (has_new_element.wait_until(head_lock, now + duration, [this] { return head.get() != get_tail(); }))
                    return pop_head();
                else
                    return nullptr;
            }
            std::unique_ptr<node> wait_pop_head()
            {
                std::unique_lock<std::mutex> head_lock(head_mutex);
                has_new_element.wait(head_lock, [this] { return head.get() != get_tail(); });
                return pop_head();
            }
            std::unique_ptr<node> try_pop_head()
            {
                std::lock_guard<std::mutex> head_lock(head_mutex);
                if (head.get() == get_tail())
                    return nullptr;
                return pop_head();
            }
        };
    }
}
#endif //LZHLIB_QUEUE_HPP

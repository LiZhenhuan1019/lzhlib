#ifndef LZHLIB_QUEUE_HPP
#define LZHLIB_QUEUE_HPP
#include <type_traits>
#include <optional>
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
                return popped ? popped->data : std::nullopt;
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
                tail->data = std::move(value);
                {
                    std::lock_guard<std::mutex> tail_lock(tail_mutex);
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

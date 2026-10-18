#ifndef LZHLIB_LIST_INDEXED_LIST_HPP
#define LZHLIB_LIST_INDEXED_LIST_HPP
#include <vector>
#include <limits>
#include <stdexcept>
#include <string>

namespace lzhlib
{
    constexpr std::size_t null_index = std::numeric_limits<std::size_t>::max();
    // circular double-linked list, all nodes are allocated in a vector,
    // located by index rather than pointer,
    // so that indexes will not be invalidated by vector reallocating.
    // Consider using another vector that supports passing arguments to ValueT when resizing so that ValueT may not be default-constructed.
    // ValueT : Object with value-semantic.
    //
    // A node that is not linked into a list has both links equal to null_index;
    // resize() appends such unlinked nodes, form_list() / push() link them.
    template <typename ValueT>
    class indexed_list
    {
        struct node
        {
            ValueT value;
            // Without these initializers a node appended by resize() would be
            // value-initialized, i.e. its links would be 0 instead of null_index,
            // and walking "forward" from it would silently step into node 0.
            std::size_t next = null_index;
            std::size_t prev = null_index;
        };
    public:
        using value_type = ValueT;
        using size_type = std::size_t;
        indexed_list() = default;
        template <typename ...Args, std::enable_if_t<std::is_constructible_v<ValueT, Args&&...>, int> = 0>
        explicit indexed_list(size_type size, Args&&...args)
            : data(size, node{{std::forward<Args>(args)...}, null_index, null_index})
        {
        }
        value_type &operator[](size_type index)
        {
            return data[index].value;
        }
        value_type const &operator[](size_type index) const
        {
            return data[index].value;
        }
        value_type &at(size_type index)
        {
            check_index(index);
            return data[index].value;
        }
        value_type const &at(size_type index) const
        {
            check_index(index);
            return data[index].value;
        }
        size_type size() const
        {
            return data.size();
        }
        void resize(size_type new_size)
        {
            data.resize(new_size);
        }
        void shrink_to_fit()
        {
            data.shrink_to_fit();
        }

        // make a list using nodes in range [begin, end) in increment order.
        // returns the head of list.
        size_type form_list(size_type begin, size_type end)
        {
            if (end <= begin || begin == null_index || end > data.size())
                throw std::invalid_argument("in lzhlib::indexed_list::form_list: argument is invalid.");
            data[begin].next = begin + 1;
            data[end - 1].prev = end - 2;
            for (std::size_t i = begin + 1; i < end - 1; ++i)
            {
                data[i].prev = i - 1;
                data[i].next = i + 1;
            }
            data[begin].prev = end - 1;
            data[end - 1].next = begin;
            return begin;
        }
        size_type push(size_type &head, size_type index)
        {
            return push_impl(head, index);
        }
        size_type pop(size_type &head)
        {
            size_type result = head, prev = data[result].prev;
            if (prev == head)  // there's only one node in list.
            {
                head = null_index; // list is empty now.
                data[result].next = data[result].prev = null_index;
            } else
            {
                head = data[head].next;
                remove_impl(result);
            }
            return result;
        }

        size_type insert(size_type &head, size_type next, size_type inserted)
        {
            if (next == head)
                return push_impl(head, inserted);
            if (next == null_index)
                return insert_impl(head, inserted);
            return insert_impl(next, inserted);
        }
        void remove(size_type &head, size_type removed)
        {
            if (head == removed)
            {
                pop(head);
                return;
            }
            remove_impl(removed);
        }
        void clear()
        {
            for (node &n : data)
            {
                n.next = n.prev = null_index;
            }
        }
        size_type previous(size_type index) const
        {
            if (index == null_index)
                return null_index;
            return data[index].prev;
        }
        size_type next(size_type index) const
        {
            if (index == null_index)
                return null_index;
            return data[index].next;
        }
    private:
        std::vector<node> data;

        // at() is the checked accessor, operator[] stays unchecked on purpose:
        // the list is used in loops where the index is known to be valid.
        void check_index(size_type index) const
        {
            if (index >= data.size())
                throw std::out_of_range("in lzhlib::indexed_list::at: index " + std::to_string(index) +
                                        " is out of range, size is " + std::to_string(data.size()) + ".");
        }

        size_type push_impl(size_type &head, size_type index)
        {
            if (head == null_index)  // list is empty.
            {
                data[index].next = data[index].prev = index;
                head = index;
                return index;
            }
            insert_impl(head, index);
            head = index;
            return index;
        }
        size_type insert_impl(size_type next, size_type inserted)
        {
            size_type prev = data[next].prev;
            data[inserted].next = next;
            data[inserted].prev = prev;
            data[next].prev = inserted;
            data[prev].next = inserted;
            return inserted;
        }
        void remove_impl(size_type removed)
        {
            size_type prev = data[removed].prev;
            size_type next = data[removed].next;
            data[prev].next = next;
            data[next].prev = prev;
            data[removed].next = data[removed].prev = null_index;
        }
    };
}
#endif //LZHLIB_LIST_INDEXED_LIST_HPP

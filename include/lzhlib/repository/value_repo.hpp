#ifndef LZHLIB_REPOSITORY_VALUE_REPO_HPP
#define LZHLIB_REPOSITORY_VALUE_REPO_HPP

#include <stdexcept>  //for std::out_of_range
#include <vector>     //for std::vector
#include <functional>
#include <optional>
#include <lzhlib/list/indexed_list.hpp>
#include "object_id.hpp"

namespace lzhlib
{
    template <class ValueT>
    class value_repo
    {
        static constexpr std::size_t null_index = max_size_t_v;
        struct data_t
        {
            std::optional<ValueT> value; // invariants : (used == true) => (value.has_value() == true)
            bool used = false;
        };
    public:
        using value_t = ValueT;
        using id_t = object_id;
        using opt_t = std::optional<value_t>;

        struct size_to_large : public std::invalid_argument
        {
            explicit size_to_large(std::size_t size)
                : invalid_argument(std::string("Size of ") + std::to_string(size) + " is to large.")
            {}
        };
        class attempt_to_use_unused_stock : public std::logic_error
        {
        public:
            explicit attempt_to_use_unused_stock(id_t id)
                : logic_error(std::string("Attempt to use unused stock whose id is ") +
                              std::to_string(id.id()) + "!")
            {}
        };

        class attempt_to_remove_nonexistent_stock : public std::logic_error
        {
        public:
            explicit attempt_to_remove_nonexistent_stock(id_t id)
                : logic_error(std::string("Attempt to remove nonexistent stock whose id is ") +
                              std::to_string(id.id()) + "!")
            {}
        };
        class index_out_of_range : public std::out_of_range
        {
        public:
            explicit index_out_of_range(id_t id)
                : out_of_range(std::string("index with value ") + std::to_string(id.id()) + "is out of range.")
            {}
        };


        value_repo() = default;
        explicit value_repo(std::size_t init_size)
            : list(init_size)
        {
            unused_head = list.form_list(0, init_size);
        }

        struct base_iter
        {
            using difference_type = std::ptrdiff_t;
            using value_type = value_t;
            using pointer = value_type *;
            using reference = value_type &;
            using iterator_category = std::bidirectional_iterator_tag;
        private:
            template <typename iter1, typename iter2>
            static constexpr bool is_iterator_v = std::is_base_of_v<base_iter, iter1> && std::is_base_of_v<base_iter, iter2>;
        public:
            template <typename iter1, typename iter2, std::enable_if_t<is_iterator_v<iter1, iter2>, int> = 0>
            friend bool operator==(iter1 const &lhs, iter2 const &rhs)
            {
                return lhs.id() == rhs.id();
            }
            template <typename iter1, typename iter2, std::enable_if_t<is_iterator_v<iter1, iter2>, int> = 0>
            friend bool operator!=(iter1 const &lhs, iter2 const &rhs)
            {
                return !(lhs == rhs);
            }
        };

        class iterator;

        class const_iterator : base_iter
        {
            friend class value_repo;

            friend iterator;

            const_iterator(value_repo const &repo, id_t id)
                : repo(repo), id_(id)
            {}
        public:
            value_t const &operator*()
            {
                return repo.get().get_object(id_);
            }
            value_t const *operator->()
            {
                return &repo.get().get_object(id_);
            }
            const_iterator &operator++()
            {
                id_ = repo.get().next_stock(id_);
                return *this;
            }
            const_iterator operator++(int)
            {
                const_iterator ret = *this;
                ++*this;
                return ret;
            }
            const_iterator &operator--()
            {
                id_ = repo.get().prev_stock(id_);
                return *this;
            }
            const_iterator operator--(int)
            {
                const_iterator ret = *this;
                --*this;
                return ret;
            }

            id_t id() const
            {
                return id_;
            }


        protected:
            std::reference_wrapper<const value_repo> repo;
            id_t id_;
        };

        class iterator : base_iter
        {
            friend class value_repo;

            iterator(value_repo &repo, id_t id)
                : repo(repo), id_(id)
            {}
        public:
            operator const_iterator()
            {
                return const_iterator(repo, id_);
            }
            value_t &operator*()
            {
                return repo.get().get_object(id_);
            }
            value_t *operator->()
            {
                return &repo.get().get_object(id_);
            }
            iterator &operator++()
            {
                id_ = repo.get().next_stock(id_);
                return *this;
            }
            iterator operator++(int)
            {
                iterator ret = *this;
                ++*this;
                return ret;
            }
            iterator &operator--()
            {
                id_ = repo.get().prev_stock(id_);
                return *this;
            }
            iterator operator--(int)
            {
                iterator ret = *this;
                --*this;
                return ret;
            }

            id_t id() const
            {
                return id_;
            }
        private:
            std::reference_wrapper<value_repo> repo;
            id_t id_;
        };


        bool is_id_used(id_t id) const
        {
            return id.id() < list.size() && !check_unused(id);
        }

        //id_ interface
        value_t &get_object(id_t id)
        {
#ifndef NDEBUG
            if (check_unused(id))
            {
                throw attempt_to_use_unused_stock(id);
            }
#endif // NDEBUG
            return *list[id.id()].value;
        }
        value_t const &get_object(id_t id) const
        {
#ifndef NDEBUG
            if (check_unused(id))
            {
                throw attempt_to_use_unused_stock(id);
            }
#endif // NDEBUG
            return *list[id.id()].value;
        }
        template <class ...Args>
        id_t add_object(Args &&... args)
        {
            if (unused_head == null_index)
            {
                std::size_t new_size = list.size(), old_size = new_size;
                if (new_size < std::numeric_limits<std::size_t>::max() / 3 * 2)
                {
                    new_size = new_size * 3 / 2 + 1;
                }
                list.resize(new_size);
                unused_head = list.form_list(old_size, new_size);
            }
            std::size_t result = list.pop(unused_head);
            list[result].value = value_t{std::forward<Args>(args)...};
            list[result].used = true;
            list.push(used_head, result);
            return result;
        }
        void remove_object(id_t id)
        {
#ifndef NDEBUG
            if (check_unused(id))
                throw attempt_to_remove_nonexistent_stock(id);
#endif // NDEBUG
            list[id.id()].value.reset();
            return_object(id);
        }
        // make object of id unused without destroying it.
        void return_object(id_t id)
        {
            list[id.id()].used = false;
            list.remove(used_head, id.id());
            list.push(unused_head, id.id());
        }

        id_t first_stock() const
        {
            return used_head;
        }
        id_t end_stock() const
        {
            return null_index;
        }
        id_t next_stock(id_t current) const
        {
#ifndef NDEBUG
            if (current.id() == null_index)
                throw index_out_of_range(current);
#endif // NDEBUG
            if (current.id() == list.previous(used_head))
                return null_index;
            return list.next(current.id());
        }
        id_t prev_stock(id_t current) const
        {
#ifndef NDEBUG
            if (current.id() == used_head)
                throw index_out_of_range(current);
#endif // NDEBUG
            if (current.id() == null_index)
                return list.previous(used_head);
            return list.previous(current.id());
        }


        //iterator interface
        iterator iter(id_t id)
        {
            return iterator(*this, id);
        }
        const_iterator iter(id_t id) const
        {
            return const_iterator(*this, id);
        }
        const_iterator citer(id_t id) const //const iter
        {
            return const_iterator(*this, id);
        }
        iterator begin()
        {
            return iterator(*this, first_stock());
        }
        const_iterator begin() const
        {
            return const_iterator(*this, first_stock());
        }
        const_iterator cbegin() const
        {
            return const_iterator(*this, first_stock());
        }
        iterator end()
        {
            return iterator(*this, id_t{null_index});
        }
        const_iterator end() const
        {
            return const_iterator(*this, id_t{null_index});
        }
        const_iterator cend() const
        {
            return const_iterator(*this, id_t{null_index});
        }
    private:


        bool check_unused(id_t id) const
        {
            return list[id.id()].used == false;
        }
    private:
        indexed_list<data_t> list;
        std::size_t unused_head = null_index;
        std::size_t used_head = null_index;

        friend std::istream &operator>>(std::istream &in, value_repo &repository)
        {
            in >> repository.list;
            in >> repository.unused_head;
            in >> repository.used_head;
            return in;
        }
        friend std::ostream &operator<<(std::ostream &out, value_repo const &repository)
        {
            out << repository.list;
            out << repository.unused_head;
            out << repository.used_head;
            return out;
        }
    };

}

#endif // LZHLIB_REPOSITORY_VALUE_REPO_HPP

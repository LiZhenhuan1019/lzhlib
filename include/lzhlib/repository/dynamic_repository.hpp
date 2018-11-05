#ifndef LZHLIB_REPOSITORY_DYNAMIC_REPOSITORY_HPP
#define LZHLIB_REPOSITORY_DYNAMIC_REPOSITORY_HPP

#include <stdexcept>  //for std::out_of_range
#include <vector>     //for std::vector
#include <functional>
#include <optional>
#include "stock_id.hpp"

namespace lzhlib
{
    template <class StockT>
    class dynamic_repository
    {
    public:
        using stock_t = StockT;
        using id_t = stock_id;
        using opt_t = std::optional<stock_t>;

        class attempt_to_use_unassigned_stock : public std::logic_error
        {
        public:
            attempt_to_use_unassigned_stock(id_t id)
                : logic_error(std::string("Attempt to use unassigned stock whose id is ") +
                              std::to_string(id.id()) + "!")
            {}
        };

        class attempt_to_remove_nonexistent_stock : public std::logic_error
        {
        public:
            attempt_to_remove_nonexistent_stock(id_t id)
                : logic_error(std::string("Attempt to use unremovable stock whose id is ") +
                              std::to_string(id.id()) + "!")
            {}
        };

        class attempt_to_reuse_unreusable_stock : public std::logic_error
        {
        public:
            attempt_to_reuse_unreusable_stock(id_t id)
                : logic_error(std::string("Attempt to reuse unreusable stock whose id is ") +
                              std::to_string(id.id()) + "!")
            {}
        };

        struct base_iter
        {
            using difference_type = std::ptrdiff_t;
            using value_type = stock_t;
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
            friend class dynamic_repository;

            friend iterator;

            const_iterator(dynamic_repository const &repo, id_t id)
                : repo(repo), id_(id)
            {}
        public:
            stock_t const &operator*()
            {
                return repo.get().get_stock(id_);
            }
            stock_t const *operator->()
            {
                return &repo.get().get_stock(id_);
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
            std::reference_wrapper<const dynamic_repository> repo;
            id_t id_;
        };

        class iterator : base_iter
        {
            friend class dynamic_repository;

            iterator(dynamic_repository &repo, id_t id)
                : repo(repo), id_(id)
            {}
        public:
            operator const_iterator()
            {
                return const_iterator(repo, id_);
            }
            stock_t &operator*()
            {
                return repo.get().get_stock(id_);
            }
            stock_t *operator->()
            {
                return &repo.get().get_stock(id_);
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
            std::reference_wrapper<dynamic_repository> repo;
            id_t id_;
        };


        bool is_id_valid(id_t id) const
        {
            return id.id() < stocks.size() && !is_nullopt(id);
        }

        //id_ interface
        stock_t &get_stock(id_t id)
        {
#ifndef NDEBUG
            if (is_nullopt(id))
            {
                throw attempt_to_use_unassigned_stock(id);
            }
#endif // NDEBUG
            return *stocks[id.id()];
        }
        stock_t const &get_stock(id_t id) const
        {
#ifndef NDEBUG
            if (is_nullopt(id))
            {
                throw attempt_to_use_unassigned_stock(id);
            }
#endif // NDEBUG
            return *stocks[id.id()];
        }
        template <class ...Args>
        id_t add_stock(Args &&... args)
        {
            id_t ret = reusable_stock();
            if (ret == stocks.size())  //no reusable stock
            {
                return allocate_stock(std::forward<Args>(args)...);
            }
            else
            {
                return reuse_stock(ret, std::forward<Args>(args)...);
            }
        }
        void remove_stock(id_t id)
        {
#ifndef NDEBUG
            if (is_nullopt(id))
                throw attempt_to_remove_nonexistent_stock(id);
#endif // NDEBUG
            stocks[id.id()].reset();
        }

        id_t first_stock() const
        {
            return stock_at_or_after(id_t{0});
        }
        id_t end_stock() const
        {
            return stocks.size();
        }
        id_t next_stock(id_t current) const        //precondition: current.id_() < stocks.size()
        {
            return stock_at_or_after(++current);   //postcondition: 设返回值为next,则stocks[next.id_()]为current代表的位置或current代表的位置之后的位置上的有效stock的左值,
        }                                          // 或者next.id_() == stocks.size()(current代表的位置之后的位置上均无有效stock
        id_t prev_stock(id_t current) const
        {
            return stock_at_or_before(--current);
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
            return iterator(*this, id_t{stocks.size()});
        }
        const_iterator end() const
        {
            return const_iterator(*this, id_t{stocks.size()});
        }
        const_iterator cend() const
        {
            return const_iterator(*this, id_t{stocks.size()});
        }
    private:
#ifndef NDEBUG
        bool is_nullopt(id_t id) const
        {
            return stocks[id.id()] == std::nullopt;
        }
#endif // NDEBUG
        template <class ...Args>
        id_t allocate_stock(Args &&... args)
        {
            stocks.emplace_back(std::in_place, std::forward<Args>(args)...);
            id_t ret{stocks.size() - 1};    //the allocated stock is at the last position in the container.
            return ret;
        }
        template <class ...Args>
        id_t reuse_stock(id_t reused, Args &&... args)//precondition:stocks[reused.id_()]必须为一个空指针
        {
            reusable_optional(reused) = std::make_optional<stock_t>(std::forward<Args>(args)...);
            return reused;
        }
        id_t reusable_stock() const               //postcondition: 设返回值为ret,则stocks[ret.id_()]为一个空指针.
        {                                         //或者ret.id_() == stocks.size()(current代表的位置及current代表的位置之后的位置上均无reusable stock
            id_t current{0};
            while (current.id() != stocks.size() && stocks[current.id()] != std::nullopt)
                ++current;
            return current;
        }
        opt_t &reusable_optional(id_t id)          //just a checker.The calling may be optimized out -- that's to say, may be inlined.
        {
#ifndef NDEBUG
            if (stocks[id.id()] != std::nullopt)
                throw attempt_to_reuse_unreusable_stock(id);
#endif // NDEBUG
            return stocks[id.id()];
        }
        id_t stock_at_or_after(id_t current) const //precondition: current.id_() <= stocks.size()
        {
            while (current.id() != stocks.size() && stocks[current.id()] == std::nullopt)
                ++current;
            return current;                       //postcondition: 设返回值为next,则stocks[next.id_()]为指向current代表的位置或current代表的位置之后的位置上的有效stock的指针的左值,
        }                                         //或者next.id_() == stocks.size()(current代表的位置及current代表的位置之后的位置上均无有效stock
        id_t stock_at_or_before(id_t current) const //precondition: current.id_() <= stocks.size()
        {
            while (current != first_stock() && stocks[current.id()] == std::nullopt)
                --current;
            return current;                       //postcondition: 设返回值为next,则stocks[next.id_()]为指向current代表的位置或current代表的位置之后的位置上的有效stock的指针的左值,
        }
    private:
        std::vector<opt_t> stocks;

        friend std::istream &operator>>(std::istream &in, dynamic_repository &repository)
        {
            in >> repository.stocks;
            return in;
        }
        friend std::ostream &operator<<(std::ostream &out, dynamic_repository const &repository)
        {
            out << repository.stocks;
            return out;
        }
    };

}

#endif // LZHLIB_REPOSITORY_DYNAMIC_REPOSITORY_HPP

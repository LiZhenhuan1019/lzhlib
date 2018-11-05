#ifndef LZHLIB_REPOSITORY_INDEXED_REPOSITORY_HPP
#define LZHLIB_REPOSITORY_INDEXED_REPOSITORY_HPP

#include <vector>     //for std::vector
#include <functional>
#include <type_traits>

namespace lzhlib
{
    namespace detail
    {
        template<typename T, bool = std::is_copy_constructible<typename std::remove_reference<T>::type>::value>
        struct move_or_copy_impl
        {
        };
        template<typename T>
        struct move_or_copy
        {
            using type = typename move_or_copy_impl<std::remove_reference_t<T>>::type;
        };

        template<typename T>
        struct move_or_copy_impl<T, true>
        {
            using type = T&;
        };

        template<typename T>
        struct move_or_copy_impl<T, false>
        {
            using type = T&&;
        };
    }

    template<typename StockT>//Stock_T should be able to be default constructed.
    class indexed_repository
    {
    public:
        using stock_t = StockT;
        using index_t = std::size_t;

    private:
        using vector_t = std::vector<stock_t>;
    public:

        class const_iterator
        {
        private:
            friend class indexed_repository;

            const_iterator(indexed_repository const& repo, index_t index)
                : repo(repo), index_(index)
            {}
        public:
            stock_t const& operator*() const
            {
                return repo.get().get_stock(index_);
            }
            stock_t const *operator->() const
            {
                return &repo.get().get_stock(index_);
            }
            const_iterator& operator++()
            {
                index_ = repo.get().next_stock(index_);
                return *this;
            }
            const_iterator operator++(int)
            {
                const_iterator ret = *this;
                ++*this;
                return ret;
            }
            const_iterator& operator--()
            {
                index_ = repo.get().prev_stock(index_);
                return *this;
            }
            const_iterator operator--(int)
            {
                const_iterator ret = *this;
                --*this;
                return ret;
            }
            bool operator==(const_iterator rhs) const
            {
                return index_ == rhs.index_;
            }
            bool operator!=(const_iterator rhs) const
            {
                return !(*this == rhs);
            }

            index_t index() const
            {
                return index_;
            }
            void set_index(index_t i)
            {
                index_ = i;
            }
        protected:
            std::reference_wrapper<const indexed_repository> repo;
            index_t index_;
        };
        class iterator:public const_iterator
        {
            friend  class indexed_repository;
            iterator(indexed_repository& repo,index_t index)
                :const_iterator(repo,index),repo(repo)
            {}
        public:
            stock_t& operator*() const
            {
                return repo.get().get_stock(index_);
            }
            stock_t* operator->() const
            {
                return &repo.get().get_stock(index_);
            }
            iterator& operator++()
            {
                index_ = repo.get().next_stock(index_);
                return *this;
            }
            iterator operator++(int)
            {
                iterator ret = *this;
                ++*this;
                return ret;
            }
            iterator& operator--()
            {
                index_ = repo.get().prev_stock(index_);
                return *this;
            }
            iterator operator--(int)
            {
                iterator ret = *this;
                --*this;
                return ret;
            }
            bool operator==(iterator rhs) const
            {
                return index_ == rhs.index_;
            }
            bool operator!=(iterator rhs) const
            {
                return !(*this == rhs);
            }
        private:
            std::reference_wrapper<indexed_repository> repo;
            using const_iterator::index_;
        };

        indexed_repository(stock_t invalid_stock_prototype = stock_t())
            : invalid_stock_prototype(std::move(invalid_stock_prototype))
        {}

        //index interface
        stock_t& get_stock(index_t id)
        {
            return stocks[id];
        }
        stock_t const& get_stock(index_t id) const
        {
            return stocks[id];
        }
        template<class ...Args>
        void set_stock(index_t index, Args&& ... args)
        {
            if (space_insufficient(index))
                add_past(index);
            change_stock(index, std::forward<Args>(args)...);
        }
        void remove_back()
        {
            stocks.pop_back();
        }
        void clear()
        {
            stocks.clear();
        }

        index_t first_stock() const
        {
            return 0;
        }
        bool stock_end(index_t current) const
        {
            return current == stocks.size();
        }
        index_t next_stock(index_t current) const        //precondition: current.id() < stocks.size()
        {
            return ++current;   //postcondition: 设返回值为next,则stocks[next.id()]为current代表的位置或current代表的位置之后的位置上的有效stock的左值,
        }                                          // 或者next.id() == stocks.size()(current代表的位置之后的位置上均无有效stock
        index_t prev_stock(index_t current) const
        {
            return --current;
        }
        index_t size() const
        {
            return stocks.size();
        }


        //iterator interface
        iterator iter(index_t index)
        {
            return iterator(stocks.begin() + index);
        }
        const_iterator iter(index_t index) const
        {
            return const_iterator(stocks.cbegin() + index);
        }
        const_iterator citer(index_t index) const //const iter
        {
            return const_iterator(stocks.cbegin() + index);
        }
        iterator begin()
        {
            return iterator(*this,0);
        }
        const_iterator begin() const
        {
            return const_iterator(*this,0);
        }
        const_iterator cbegin() const
        {
            return const_iterator(*this, 0);
        }
        iterator end()
        {
            return iterator(*this,stocks.size());
        }
        const_iterator end() const
        {
            return const_iterator(*this,stocks.size());
        }
        const_iterator cend() const
        {
            return const_iterator(*this,stocks.size());
        }
    private:
        bool space_insufficient(index_t index)
        {
            return stocks.size() <= index;
        }
        void add_past(index_t index)
        {
            stocks.reserve(index + 1);
            std::size_t number_of_inserted = index + 1 - stocks.size();
            for (std::size_t i = 0; i < number_of_inserted; ++i)
                stocks.emplace_back(std::forward<typename detail::move_or_copy<stock_t>::type>(invalid_stock_prototype));
        }
        template<class ...Args>
        void change_stock(index_t index, Args&& ... args)
        {
            get_stock(index) = {std::forward<Args>(args)...};
        }
    private:
        vector_t stocks;
        stock_t invalid_stock_prototype;
    };
}
#endif //LZHLIB_REPOSITORY_INDEXED_REPOSITORY_HPP

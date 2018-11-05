#ifndef LZHLIB_REPOSITIORY_STABLE_REPOSITORY_HPP
#define LZHLIB_REPOSITIORY_STABLE_REPOSITORY_HPP

#include <vector>     //for std::vector
#include <optional>
#include "./stock_id.hpp"

namespace lzhlib
{
    template<class StockT>
    class stable_repository
    {
    public:
        using stock_t = StockT;
        using id_t = stock_id;
        using pointer_t = std::optional<stock_t>;

        class const_iterator
        {
            friend class stable_repository;

            const_iterator(stable_repository const& repo, id_t id)
                : repo(repo), id(id)
            {}
        public:
            stock_t const& operator*()
            {
                return repo.get().get_stock(id);
            }
            stock_t const *operator->()
            {
                return &repo.get().get_stock(id);
            }
            const_iterator& operator++()
            {
                id = repo.get().next_stock(id);
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
                id = repo.get().prev_stock(id);
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
                return id == rhs.id;
            }
            bool operator!=(const_iterator rhs) const
            {
                return !(*this == rhs);
            }

        protected:
            std::reference_wrapper<const stable_repository> repo;
            id_t id;
        };

        class iterator : public const_iterator
        {
            friend class stable_repository;

            iterator(stable_repository& repo, id_t id)
                : const_iterator(repo, id), repo(repo)
            {}
        public:
            stock_t& operator*()
            {
                return repo.get().get_stock(id);
            }
            stock_t *operator->()
            {
                return &repo.get().get_stock(id);
            }
            iterator& operator++()
            {
                id = repo.get().next_stock(id);
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
                id = repo.get().prev_stock(id);
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
                return id == rhs.id;
            }
            bool operator!=(iterator rhs) const
            {
                return !(*this == rhs);
            }
        private:
            std::reference_wrapper<stable_repository> repo;
            using const_iterator::id;
        };


        //id interface
        stock_t& get_stock(id_t id)
        {
            return *stocks[id.id()];
        }
        stock_t const& get_stock(id_t id) const
        {
            return *stocks[id.id()];
        }
        template<class ...Args>
        id_t add_stock(Args&& ... args)
        {
            return allocate_stock(std::forward<Args>(args)...);
        }
        void remove_back()
        {
            stocks.pop_back();
        }
        void clear()
        {
            stocks.clear();
        }

        id_t first_stock() const
        {
            return id_t{0};
        }
        bool stock_end(id_t current) const
        {
            return current.id() == stocks.size();
        }
        id_t next_stock(id_t current) const        //precondition: current.id() < stocks.size()
        {
            return ++current;   //postcondition: 设返回值为next,则stocks[next.id()]为current代表的位置或current代表的位置之后的位置上的有效stock的左值,
        }                                          // 或者next.id() == stocks.size()(current代表的位置之后的位置上均无有效stock
        id_t prev_stock(id_t current) const
        {
            return --current;
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
        template<class ...Args>
        id_t allocate_stock(Args&& ... args)
        {
            stocks.push_back(std::make_optional<stock_t>(std::forward<Args>(args)...));
            id_t ret{stocks.size() - 1};    //the allocated stock is at the last position in the container.
            return ret;
        }
    private:
        std::vector<pointer_t> stocks;
    };

}

#endif //LZHLIB_REPOSITIORY_STABLE_REPOSITORY_HPP

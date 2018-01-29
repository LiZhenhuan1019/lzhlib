#ifndef LZHLIB_REPOSITIORY_LIGHT_WEIGHT_REPOSITORY_HPP
#define LZHLIB_REPOSITIORY_LIGHT_WEIGHT_REPOSITORY_HPP

#include <vector>     //for std::vector
#include "./stock_id.hpp"

namespace lzhlib
{
    template<class StockT>
    class light_weight_repository
    {
    public:
        using stock_t = StockT;
        using id_t = stock_id;

        using vector_t = std::vector<stock_t>;
        using const_iterator = typename vector_t::const_iterator;
        using iterator = typename vector_t::iterator;


        //id interface
        stock_t& get_stock(id_t id)
        {
            return stocks[id.id()];
        }
        stock_t const& get_stock(id_t id) const
        {
            return stocks[id.id()];
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
            return iterator(stocks.begin() + id);
        }
        const_iterator iter(id_t id) const
        {
            return const_iterator(stocks.begin() + id);
        }
        const_iterator citer(id_t id) const //const iter
        {
            return const_iterator(stocks.cbegin() + id);
        }
        iterator begin()
        {
            return iterator(stocks.begin());
        }
        const_iterator begin() const
        {
            return const_iterator(stocks.begin());
        }
        const_iterator cbegin() const
        {
            return const_iterator(stocks.cbegin());
        }
        iterator end()
        {
            return iterator(stocks.end());
        }
        const_iterator end() const
        {
            return const_iterator(stocks.end());
        }
        const_iterator cend() const
        {
            return const_iterator(stocks.cend());
        }
    private:
        template<class ...Args>
        id_t allocate_stock(Args&& ... args)
        {
            stocks.emplace_back(std::forward<Args>(args)...);
            id_t ret{stocks.size() - 1};    //the allocated stock is at the last position in the container.
            return ret;
        }
    private:
        vector_t stocks;
    };

}
#endif //LZHLIB_REPOSITIORY_LIGHT_WEIGHT_REPOSITORY_HPP

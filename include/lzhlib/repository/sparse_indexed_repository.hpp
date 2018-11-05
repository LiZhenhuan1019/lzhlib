#ifndef LZHLIB_REPOSITIORY_SPARSE_INDEXED_REPOSITORY_HPP
#define LZHLIB_REPOSITIORY_SPARSE_INDEXED_REPOSITORY_HPP

#include <map>
namespace lzhlib
{
    template <typename StockT>
    class sparse_indexed_repository
    {
    public:
        using stock_t = StockT;
        using index_t = std::size_t;

    private:
        using underlying_container = std::map<index_t,stock_t>;
    public:
        using const_iterator = typename underlying_container::const_iterator;
        using iterator = typename underlying_container::iterator;


        //index interface
        stock_t& get_stock(index_t id)
        {
            return container.at(id);
        }
        stock_t const& get_stock(index_t id) const
        {
            return container.at(id);
        }
        template<class ...Args>
        void set_stock(index_t index, Args&& ... args)
        {
            container.insert_or_assign(index,stock_t(std::forward<Args>(args)...));
        }
        void clear()
        {
            container.clear();
        }


        //iterator interface
        iterator iter(index_t index)
        {
            return container.find(index);
        }
        const_iterator iter(index_t index) const
        {
            return container.find(index);
        }
        const_iterator citer(index_t index) const //const iter
        {
            return container.find(index);
        }
        iterator begin()
        {
            return container.begin();
        }
        const_iterator begin() const
        {
            return container.begin();
        }
        const_iterator cbegin() const
        {
            return container.cbegin();
        }
        iterator end()
        {
            return container.end();
        }
        const_iterator end() const
        {
            return container.end();
        }
        const_iterator cend() const
        {
            return container.cend();
        }
    private:
        underlying_container container;
    };
}
#endif //LZHLIB_REPOSITIORY_SPARSE_INDEXED_REPOSITORY_HPP

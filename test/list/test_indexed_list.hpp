#ifndef LZHLIB_TEST_INDEXED_LIST_HPP
#define LZHLIB_TEST_INDEXED_LIST_HPP
#include <string>
#include <cassert>
#include "lzhlib/list/indexed_list.hpp"
namespace lzhlib::test::list::indexed_list
{
    void test()
    {
        lzhlib::indexed_list<std::string> list(20);
        std::size_t head = null_index;

        for (std::size_t i = 0; i < 10; ++i)
        {
            std::size_t old_head = head;
            list.push(head, i);
            assert(head == i);
            assert(list.next(i) == (i ? old_head : 0));
            assert(list.previous(i) == 0);
        }
        // 9 -> 8 -> ... 2 -> 1 -> 0
        // ^                       |
        // |                       |
        //  <----------------------

        for (std::size_t i = 10; i < 20; ++i)
        {
            list.insert(head, null_index, i);
            assert(list.previous(i) == (i == 10 ? 0 : i - 1));
            assert(list.next(i) == 9);
        }
        // 9 -> 8 -> ... -> 1 -> 0 -> 10 -> 11 ... -> 18 -> 19
        // ^                                                 |
        // |                                                 |
        //  <------------------------------------------------

        for (std::size_t i = 10; i > 0; --i)
        {
            assert(list.next(i - 1) == (i == 1 ? 10 : i - 2));
            assert(list.previous(i - 1) == 19);
            std::size_t result = list.pop(head);
            assert(result == i - 1);
        }
        // 10 -> 11 ... -> 18 -> 19
        // ^                      |
        // |                      |
        //  <---------------------
        for (std::size_t i = 20; i > 10; --i)
        {
            assert(list.next(i - 1) == 10);
            assert(list.previous(i - 1) == (i == 11 ? 10 : i - 2));
            list.remove(head, i - 1);
        }

    }
}
#endif //LZHLIB_TEST_INDEXED_LIST_HPP

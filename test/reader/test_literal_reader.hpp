#ifndef LZHLIB_TEST_LITERAL_READER_HPP
#define LZHLIB_TEST_LITERAL_READER_HPP
#include <functional>
#include "test_check.hpp"
#include <lzhlib/reader/literal_reader.hpp>
namespace lzhlib::test::reader::literal_reader
{
    namespace
    {
        inline int f(std::string_view)
        {
            return 1;
        }
    }
    inline void test()
    {
        using lzhlib::reader::type;
        using lzhlib::reader::type_list;
        using lzhlib::tuple::arg;
        using reader = lzhlib::reader::literal_reader<type_list<type<int>,
                                                                type<std::string>,
                                                                type<int, std::function<int(std::string_view)>>>>;
        reader c1(arg<2>(f));
        LZH_TEST_CHECK(c1.read<0>("2") == 2);
        LZH_TEST_CHECK(c1.read<1>("\"hello\"") == "hello");
        LZH_TEST_CHECK(c1.read<2>("") == 1);
    }
}
#endif //LZHLIB_TEST_LITERAL_READER_HPP

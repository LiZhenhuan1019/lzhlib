#ifndef LZHLIB_TEST_TUPLE_EXT_HPP
#define LZHLIB_TEST_TUPLE_EXT_HPP
#include <vector>
#include <cassert>
#include "lzhlib/stdlib_ext/tuple_ext.hpp"
namespace lzhlib::test
{
    namespace stdlib_ext
    {
        namespace tuple_ext
        {
            inline void test()
            {
                std::tuple<int, std::string, std::string, std::vector<std::string>> t1 = {0, "hello", "", {"string 1", "string 2"}};
                using lzhlib::tuple::arg;
                using namespace std::literals;
                auto t2 = lzhlib::tuple::create_tuple<decltype(t1)>(arg<1>("hello"), arg<3>({"string 1"s, "string 2"s}));
                static_assert(std::is_same_v<decltype(t1), decltype(t2)>, "test failed : type does not match.");
                assert(t1 == t2);
            }
        }
    }
}
#endif //LZHLIB_TEST_TUPLE_EXT_HPP

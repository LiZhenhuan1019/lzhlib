#ifndef LZHLIB_TEST_TUPLE_EXT_HPP
#define LZHLIB_TEST_TUPLE_EXT_HPP
#include <vector>
#include <cassert>
#include "lzhlib/utility/tuple_ext.hpp"
namespace lzhlib::test
{
    namespace utility
    {
        namespace tuple_ext
        {
            namespace detail
            {
                struct not_default_destructible
                {
                    explicit not_default_destructible(int i)
                        : i(i)
                    {}
                    int i;
                    friend bool operator==(not_default_destructible const &lhs, not_default_destructible const &rhs)
                    {
                        return lhs.i == rhs.i;
                    }
                };
            }
            inline void test()
            {
                std::tuple<int, std::string, std::string, std::vector<std::string>, detail::not_default_destructible> t1 = {0, "hello", "", {"string 1", "string 2"}, detail::not_default_destructible{1}};
                using lzhlib::tuple::arg;
                using namespace std::literals;
                auto t2 = lzhlib::tuple::create_tuple<decltype(t1)>(arg<1>("hello"), arg<3>({"string 1"s, "string 2"s}), arg<4>(1));
                static_assert(std::is_same_v<decltype(t1), decltype(t2)>, "test failed : type does not match.");
                assert(t1 == t2);
            }
        }
    }
}
#endif //LZHLIB_TEST_TUPLE_EXT_HPP

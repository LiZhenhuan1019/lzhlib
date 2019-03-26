#ifndef LZHLIB_TEST_UTILITY_HPP
#define LZHLIB_TEST_UTILITY_HPP
#include "test_tuple_ext.hpp"
#include "test_optional.hpp"
namespace lzhlib::test
{
    namespace utility
    {
        inline void test()
        {
            tuple_ext::test();
            optional::test();
        }
    }
}
#endif //LZHLIB_TEST_UTILITY_HPP

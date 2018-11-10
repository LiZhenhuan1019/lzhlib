#ifndef LZHLIB_TEST_READER_HPP
#define LZHLIB_TEST_READER_HPP
#include "test_literal_reader.hpp"
#include "test_dynamic_reader.hpp"
namespace lzhlib::test
{
    namespace reader
    {
        inline void test()
        {
            literal_reader::test();
            dynamic_reader::test();
        }
    }
}
#endif //LZHLIB_TEST_READER_HPP

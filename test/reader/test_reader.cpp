#include "test_check.hpp"

#include "test_reader.hpp"


int main()
{
    return lzhlib::test::run("test_reader", lzhlib::test::reader::test);
}

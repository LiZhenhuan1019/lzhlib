#include "test_check.hpp"

#include "test_thread.hpp"


int main()
{
    return lzhlib::test::run("test_thread", lzhlib::test::thread::test);
}

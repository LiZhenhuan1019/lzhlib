#include "test_check.hpp"

#include "test_repository.hpp"


int main()
{
    return lzhlib::test::run("test_repository", lzhlib::test::repository::test);
}

#include <iostream>
#include <cassert>

#include "graph/test_graph.hpp"
#include "repository/test_repository.hpp"


namespace test
{
    void test()
    {
        graph::test();
        repository::test();
    }
}

#ifdef LZHLIB_BUILDING_AND_TESTING
int main()
{
    test::test();
    std::cout << "Success!成功" << std::endl;
    return 0;
}
#endif

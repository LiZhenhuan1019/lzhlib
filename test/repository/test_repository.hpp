#ifndef LZHLIB_REPOSITORY_TEST_REPOSITORY_HPP
#define LZHLIB_REPOSITORY_TEST_REPOSITORY_HPP
#include <cassert>
#include "lzhlib/repository/repository.hpp"
namespace test
{
    namespace repository
    {

        namespace dynamic
        {
            namespace detail
            {
                void generate_discontinous_repo(lzhlib::dynamic_repository<std::string> &repo)
                {
                    auto a = repo.add_stock("a");
                    repo.add_stock("b");
                    auto c = repo.add_stock("c");
                    repo.add_stock("d");
                    repo.add_stock("e");
                    auto e = repo.add_stock("f");
                    repo.add_stock("g");
                    auto h = repo.add_stock("h");
                    repo.remove_stock(a);
                    repo.remove_stock(c);
                    repo.remove_stock(e);
                    repo.remove_stock(h);

                }
            }
            void test_accessor()
            {
                lzhlib::dynamic_repository<std::string> repo;

                auto s0 = repo.add_stock("a");
                auto s1 = repo.add_stock("b");

                assert(repo.get_stock(s0) == "a");
                assert(repo.get_stock(s1) == "b");

                repo.get_stock(s0) = "c";
                repo.get_stock(s1) = "d";
                assert(repo.get_stock(s0) == "c");
                assert(repo.get_stock(s1) == "d");
            }
            void test_iter_by_id()
            {
                lzhlib::dynamic_repository<std::string> repo;
                detail::generate_discontinous_repo(repo);

                lzhlib::stock_id id = repo.first_stock();
                {
                    std::vector<std::string> stocks;
                    for (; id != repo.end_stock(); id = repo.next_stock(id))
                        stocks.push_back(repo.get_stock(id));
                    std::vector<std::string> vertifier{"b", "d", "e", "g"};
                    assert(stocks == vertifier);
                }
                {
                    std::vector<std::string> stocks;
                    id = repo.prev_stock(id);
                    for (; id != repo.first_stock(); id = repo.prev_stock(id))
                        stocks.push_back(repo.get_stock(id));
                    stocks.push_back(repo.get_stock(id));
                    std::vector<std::string> reversed_vertifier{"g", "e", "d", "b"};
                    assert(stocks == reversed_vertifier);
                }
            }
            void test_iterator()
            {
                lzhlib::dynamic_repository<std::string> repo;
                detail::generate_discontinous_repo(repo);
                {
                    std::vector<std::string> stocks;
                    for (auto iter = repo.begin(); iter != repo.end(); ++iter)
                        stocks.push_back(*iter);
                    std::vector<std::string> vertifier{"b", "d", "e", "g"};
                    assert(stocks == vertifier);
                }
                {
                    std::vector<std::string> stocks;
                    auto iter = repo.end();
                    for (--iter; iter != repo.begin(); --iter)
                        stocks.push_back(*iter);
                    stocks.push_back(*iter);
                    std::vector<std::string> reversed_vertifier{"g", "e", "d", "b"};
                    assert(stocks == reversed_vertifier);
                }
                {
                    std::vector<std::string> stocks;
                    for (auto &str:repo)
                        stocks.push_back(str);
                    std::vector<std::string> vertifier{"b", "d", "e", "g"};
                    assert(stocks == vertifier);
                }
            }
        }
        namespace stable
        {
            namespace detail
            {
                void generate_discontinous_repo(lzhlib::stable_repository<std::string> &repo)
                {
                    repo.add_stock("a");
                    repo.add_stock("b");
                    repo.add_stock("c");
                    repo.add_stock("d");
                    repo.add_stock("e");
                    repo.add_stock("f");
                    repo.add_stock("g");
                    repo.add_stock("h");

                }
            }
            void test_accessor()
            {
                lzhlib::stable_repository<std::string> repo;

                auto s0 = repo.add_stock("a");
                auto s1 = repo.add_stock("b");

                assert(repo.get_stock(s0) == "a");
                assert(repo.get_stock(s1) == "b");

                repo.get_stock(s0) = "c";
                repo.get_stock(s1) = "d";
                assert(repo.get_stock(s0) == "c");
                assert(repo.get_stock(s1) == "d");
            }
            void test_iter_by_id()
            {
                lzhlib::stable_repository<std::string> repo;
                detail::generate_discontinous_repo(repo);

                lzhlib::stock_id id = repo.first_stock();
                {
                    std::vector<std::string> stocks;
                    for (; !repo.stock_end(id); id = repo.next_stock(id))
                        stocks.push_back(repo.get_stock(id));
                    std::vector<std::string> vertifier{"a", "b", "c", "d", "e", "f", "g", "h"};
                    assert(stocks == vertifier);
                }
                {
                    std::vector<std::string> stocks;
                    id = repo.prev_stock(id);
                    for (; id != repo.first_stock(); id = repo.prev_stock(id))
                        stocks.push_back(repo.get_stock(id));
                    stocks.push_back(repo.get_stock(id));
                    std::vector<std::string> reversed_vertifier{"h", "g", "f", "e", "d", "c", "b", "a"};
                    assert(stocks == reversed_vertifier);
                }
            }
            void test_iterator()
            {
                lzhlib::stable_repository<std::string> repo;
                detail::generate_discontinous_repo(repo);
                {
                    std::vector<std::string> stocks;
                    for (auto iter = repo.begin(); iter != repo.end(); ++iter)
                        stocks.push_back(*iter);
                    std::vector<std::string> vertifier{"a", "b", "c", "d", "e", "f", "g", "h"};
                    assert(stocks == vertifier);
                }
                {
                    std::vector<std::string> stocks;
                    auto iter = repo.end();
                    for (--iter; iter != repo.begin(); --iter)
                        stocks.push_back(*iter);
                    stocks.push_back(*iter);
                    std::vector<std::string> reversed_vertifier{"h", "g", "f", "e", "d", "c", "b", "a"};
                    assert(stocks == reversed_vertifier);
                }
                {
                    std::vector<std::string> stocks;
                    for (auto &str:repo)
                        stocks.push_back(str);
                    std::vector<std::string> vertifier{"a", "b", "c", "d", "e", "f", "g", "h"};
                    assert(stocks == vertifier);
                }
            }
        }
        namespace light_weight
        {
            namespace detail
            {
                void generate_discontinous_repo(lzhlib::light_weight_repository<std::string> &repo)
                {
                    repo.add_stock("a");
                    repo.add_stock("b");
                    repo.add_stock("c");
                    repo.add_stock("d");
                    repo.add_stock("e");
                    repo.add_stock("f");
                    repo.add_stock("g");
                    repo.add_stock("h");

                }
            }
            void test_accessor()
            {
                lzhlib::light_weight_repository<std::string> repo;

                auto s0 = repo.add_stock("a");
                auto s1 = repo.add_stock("b");

                assert(repo.get_stock(s0) == "a");
                assert(repo.get_stock(s1) == "b");

                repo.get_stock(s0) = "c";
                repo.get_stock(s1) = "d";
                assert(repo.get_stock(s0) == "c");
                assert(repo.get_stock(s1) == "d");
            }
            void test_iter_by_id()
            {
                lzhlib::light_weight_repository<std::string> repo;
                detail::generate_discontinous_repo(repo);

                lzhlib::stock_id id = repo.first_stock();
                {
                    std::vector<std::string> stocks;
                    for (; !repo.stock_end(id); id = repo.next_stock(id))
                        stocks.push_back(repo.get_stock(id));
                    std::vector<std::string> vertifier{"a", "b", "c", "d", "e", "f", "g", "h"};
                    assert(stocks == vertifier);
                }
                {
                    std::vector<std::string> stocks;
                    id = repo.prev_stock(id);
                    for (; id != repo.first_stock(); id = repo.prev_stock(id))
                        stocks.push_back(repo.get_stock(id));
                    stocks.push_back(repo.get_stock(id));
                    std::vector<std::string> reversed_vertifier{"h", "g", "f", "e", "d", "c", "b", "a"};
                    assert(stocks == reversed_vertifier);
                }
            }
            void test_iterator()
            {
                lzhlib::light_weight_repository<std::string> repo;
                detail::generate_discontinous_repo(repo);
                {
                    std::vector<std::string> stocks;
                    for (auto iter = repo.begin(); iter != repo.end(); ++iter)
                        stocks.push_back(*iter);
                    std::vector<std::string> vertifier{"a", "b", "c", "d", "e", "f", "g", "h"};
                    assert(stocks == vertifier);
                }
                {
                    std::vector<std::string> stocks;
                    auto iter = repo.end();
                    for (--iter; iter != repo.begin(); --iter)
                        stocks.push_back(*iter);
                    stocks.push_back(*iter);
                    std::vector<std::string> reversed_vertifier{"h", "g", "f", "e", "d", "c", "b", "a"};
                    assert(stocks == reversed_vertifier);
                }
                {
                    std::vector<std::string> stocks;
                    for (auto &str:repo)
                        stocks.push_back(str);
                    std::vector<std::string> vertifier{"a", "b", "c", "d", "e", "f", "g", "h"};
                    assert(stocks == vertifier);
                }
            }
        }
        namespace indexed
        {
            namespace detail
            {
                void generate_discontinous_repo(lzhlib::indexed_repository<std::string> &repo)
                {
                    repo.set_stock(3, "d");
                    repo.set_stock(5, "f");
                    repo.set_stock(4, "e");
                    repo.set_stock(6, "g");
                    repo.set_stock(7, "h");
                    repo.set_stock(2, "c");
                    repo.set_stock(0, "a");
                    repo.set_stock(1, "b");

                }
            }
            void test_accessor()
            {
                lzhlib::indexed_repository<std::string> repo;

                repo.set_stock(1, "b");
                repo.set_stock(0, "a");

                assert(repo.get_stock(0) == "a");
                assert(repo.get_stock(1) == "b");

                repo.get_stock(0) = "c";
                repo.get_stock(1) = "d";
                assert(repo.get_stock(0) == "c");
                assert(repo.get_stock(1) == "d");
            }
            void test_iter_by_id()
            {
                lzhlib::indexed_repository<std::string> repo;
                detail::generate_discontinous_repo(repo);

                auto id = repo.first_stock();
                {
                    std::vector<std::string> stocks;
                    for (; !repo.stock_end(id); id = repo.next_stock(id))
                        stocks.push_back(repo.get_stock(id));
                    std::vector<std::string> vertifier{"a", "b", "c", "d", "e", "f", "g", "h"};
                    assert(stocks == vertifier);
                }
                {
                    std::vector<std::string> stocks;
                    id = repo.prev_stock(id);
                    for (; id != repo.first_stock(); id = repo.prev_stock(id))
                        stocks.push_back(repo.get_stock(id));
                    stocks.push_back(repo.get_stock(id));
                    std::vector<std::string> reversed_vertifier{"h", "g", "f", "e", "d", "c", "b", "a"};
                    assert(stocks == reversed_vertifier);
                }
            }
            void test_iterator()
            {
                lzhlib::indexed_repository<std::string> repo;
                detail::generate_discontinous_repo(repo);
                {
                    std::vector<std::string> stocks;
                    for (auto iter = repo.begin(); iter != repo.end(); ++iter)
                        stocks.push_back(*iter);
                    std::vector<std::string> vertifier{"a", "b", "c", "d", "e", "f", "g", "h"};
                    assert(stocks == vertifier);
                }
                {
                    std::vector<std::string> stocks;
                    auto iter = repo.end();
                    for (--iter; iter != repo.begin(); --iter)
                        stocks.push_back(*iter);
                    stocks.push_back(*iter);
                    std::vector<std::string> reversed_vertifier{"h", "g", "f", "e", "d", "c", "b", "a"};
                    assert(stocks == reversed_vertifier);
                }
                {
                    std::vector<std::string> stocks;
                    for (auto &str:repo)
                        stocks.push_back(str);
                    std::vector<std::string> vertifier{"a", "b", "c", "d", "e", "f", "g", "h"};
                    assert(stocks == vertifier);
                }
            }
        }
        void test_dynamic_repository()
        {
            using namespace dynamic;
            test_accessor();
            test_iter_by_id();
            test_iterator();
        }
        void test_stable_repository()
        {
            using namespace stable;
            test_accessor();
            test_iter_by_id();
            test_iterator();
        }
        void test_light_weight_repository()
        {
            using namespace light_weight;
            test_accessor();
            test_iter_by_id();
            test_iterator();
        }
        void test_indexed_repository()
        {
            using namespace indexed;
            test_accessor();
            test_iter_by_id();
            test_iterator();
        }
        void test()
        {
            test_dynamic_repository();
            test_stable_repository();
            test_light_weight_repository();
            test_indexed_repository();
        }
    }
}
#endif //LZHLIB_REPOSITORY_TEST_REPOSITORY_HPP

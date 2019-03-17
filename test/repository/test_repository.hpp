#ifndef LZHLIB_REPOSITORY_TEST_REPOSITORY_HPP
#define LZHLIB_REPOSITORY_TEST_REPOSITORY_HPP
#include <cassert>
#include <map>
#include "lzhlib/repository/repository.hpp"

namespace lzhlib::test
{
    namespace repository
    {
        namespace value
        {
            namespace detail
            {
                void generate_discontinous_repo(lzhlib::value_repo<std::string> &repo)
                {
                    auto a = repo.add_object("a");
                    repo.add_object("b");
                    auto c = repo.add_object("c");
                    repo.add_object("d");
                    repo.add_object("e");
                    auto e = repo.add_object("f");
                    repo.add_object("g");
                    auto h = repo.add_object("h");
                    repo.remove_object(a);
                    repo.remove_object(c);
                    repo.remove_object(e);
                    repo.remove_object(h);

                }
            }
            void test_accessor()
            {
                lzhlib::value_repo<std::string> repo;

                auto s0 = repo.add_object("a");
                auto s1 = repo.add_object("b");

                assert(repo.get_object(s0) == "a");
                assert(repo.get_object(s1) == "b");

                repo.get_object(s0) = "c";
                repo.get_object(s1) = "d";
                assert(repo.get_object(s0) == "c");
                assert(repo.get_object(s1) == "d");
            }
            void test_iter_by_id()
            {
                lzhlib::value_repo<std::string> repo;
                detail::generate_discontinous_repo(repo);

                lzhlib::object_id id = repo.first_stock();
                std::map<std::string, int> verifier{{"b", 0},
                                                    {"d", 0},
                                                    {"e", 0},
                                                    {"g", 0}};
                for (; id != repo.end_stock(); id = repo.next_stock(id))
                {
                    verifier[repo.get_object(id)]++;
                    assert(verifier[repo.get_object(id)] == 1);
                }
                for (id = repo.prev_stock(repo.end_stock()); id != repo.first_stock(); id = repo.prev_stock(id))
                {
                    verifier[repo.get_object(id)]++;
                    assert(verifier[repo.get_object(id)] == 2);
                }
                verifier[repo.get_object(id)]++;
                assert(verifier[repo.get_object(id)] == 2);
                for (auto const &each : verifier)
                {
                    assert(each.second == 2);
                }
            }
            void test_iterator()
            {
                lzhlib::value_repo<std::string> repo;
                detail::generate_discontinous_repo(repo);
                std::map<std::string, int> verifier{{"b", 0},
                                                    {"d", 0},
                                                    {"e", 0},
                                                    {"g", 0}};
                for (auto iter = repo.begin(); iter != repo.end(); ++iter)
                {
                    verifier[*iter]++;
                    assert(verifier[*iter] == 1);
                }
                {
                    auto iter = repo.end();
                    for (--iter; iter != repo.begin(); --iter)
                    {
                        verifier[*iter]++;
                        assert(verifier[*iter] == 2);
                    }

                    verifier[*iter]++;
                    assert(verifier[*iter] == 2);
                }
                for (auto &str:repo)
                {
                    verifier[str]++;
                    assert(verifier[str] == 3);
                }

                for (auto const &each : verifier)
                {
                    assert(each.second == 3);
                }
            }
        }
        void test_value_repo()
        {
            using namespace value;
            test_accessor();
            test_iter_by_id();
            test_iterator();
        }
        void test()
        {
            test_value_repo();
        }
    }
}
#endif //LZHLIB_REPOSITORY_TEST_REPOSITORY_HPP

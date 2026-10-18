#ifndef LZHLIB_REPOSITORY_TEST_REPOSITORY_HPP
#define LZHLIB_REPOSITORY_TEST_REPOSITORY_HPP
#include "test_check.hpp"
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
                    auto a = repo.assign_object("a");
                    repo.assign_object("b");
                    auto c = repo.assign_object("c");
                    repo.assign_object("d");
                    repo.assign_object("e");
                    auto e = repo.assign_object("f");
                    repo.assign_object("g");
                    auto h = repo.assign_object("h");
                    repo.remove_object(a);
                    repo.remove_object(c);
                    repo.remove_object(e);
                    repo.remove_object(h);

                }
            }
            void test_accessor()
            {
                lzhlib::value_repo<std::string> repo;

                auto s0 = repo.assign_object("a");
                auto s1 = repo.assign_object("b");

                LZH_TEST_CHECK(repo.get_object(s0) == "a");
                LZH_TEST_CHECK(repo.get_object(s1) == "b");

                repo.get_object(s0) = "c";
                repo.get_object(s1) = "d";
                LZH_TEST_CHECK(repo.get_object(s0) == "c");
                LZH_TEST_CHECK(repo.get_object(s1) == "d");
            }
            void test_iter_by_id()
            {
                lzhlib::value_repo<std::string> repo;
                detail::generate_discontinous_repo(repo);

                lzhlib::object_id id = repo.first_id();
                std::map<std::string, int> verifier{{"b", 0},
                                                    {"d", 0},
                                                    {"e", 0},
                                                    {"g", 0}};
                for (; id != repo.end_id(); id = repo.next_id(id))
                {
                    verifier[repo.get_object(id)]++;
                    LZH_TEST_CHECK(verifier[repo.get_object(id)] == 1);
                }
                for (id = repo.prev_id(repo.end_id()); id != repo.first_id(); id = repo.prev_id(id))
                {
                    verifier[repo.get_object(id)]++;
                    LZH_TEST_CHECK(verifier[repo.get_object(id)] == 2);
                }
                verifier[repo.get_object(id)]++;
                LZH_TEST_CHECK(verifier[repo.get_object(id)] == 2);
                for (auto const &each : verifier)
                {
                    LZH_TEST_CHECK(each.second == 2);
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
                    LZH_TEST_CHECK(verifier[*iter] == 1);
                }
                {
                    auto iter = repo.end();
                    for (--iter; iter != repo.begin(); --iter)
                    {
                        verifier[*iter]++;
                        LZH_TEST_CHECK(verifier[*iter] == 2);
                    }

                    verifier[*iter]++;
                    LZH_TEST_CHECK(verifier[*iter] == 2);
                }
                for (auto &str:repo)
                {
                    verifier[str]++;
                    LZH_TEST_CHECK(verifier[str] == 3);
                }

                for (auto const &each : verifier)
                {
                    LZH_TEST_CHECK(each.second == 3);
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

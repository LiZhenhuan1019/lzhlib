#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "lzhlib/repository/value_repo.hpp"

namespace
{
    using lzhlib::value_repo;
}

TEST(value_repo, empty_repository_has_no_used_slots)
{
    value_repo<int> repo(4);

    EXPECT_FALSE(repo.is_id_used(repo.first_id()));
    EXPECT_EQ(repo.first_id().id(), lzhlib::null_index);
}

TEST(value_repo, assign_get_and_remove)
{
    value_repo<int> repo(2);

    auto first = repo.assign_object(10);
    auto second = repo.assign_object(20);

    EXPECT_TRUE(repo.is_id_used(first));
    EXPECT_TRUE(repo.is_id_used(second));
    EXPECT_EQ(repo.get_object(first), 10);
    EXPECT_EQ(repo.get_object(second), 20);

    repo.remove_object(first);
    EXPECT_FALSE(repo.is_id_used(first));
    EXPECT_TRUE(repo.is_id_used(second));
}

TEST(value_repo, iteration_visits_every_live_object_once)
{
    value_repo<int> repo(4);

    std::vector<lzhlib::object_id> ids;
    for (int i = 1; i <= 4; ++i)
    {
        ids.push_back(repo.assign_object(i * 10));
    }
    repo.remove_object(ids[1]);
    repo.remove_object(ids[3]);

    int sum = 0;
    std::size_t count = 0;
    for (auto const &value : repo)
    {
        sum += value;
        ++count;
    }

    EXPECT_EQ(count, 2u);
    EXPECT_EQ(sum, 10 + 30);
}

TEST(value_repo, repository_grows_and_keeps_existing_values)
{
    value_repo<int> repo(2);

    for (int i = 0; i < 100; ++i)
    {
        repo.assign_object(i);
    }

    int expected = 0;
    for (auto const &value : repo)
    {
        expected += value;
    }

    EXPECT_EQ(expected, 4950);
}

TEST(value_repo, reuses_returned_slots)
{
    value_repo<std::string> repo(1);

    auto id = repo.assign_object("first");
    EXPECT_EQ(repo.get_object(id), "first");

    repo.remove_object(id);
    EXPECT_FALSE(repo.is_id_used(id));

    auto reused = repo.assign_object("second");
    EXPECT_TRUE(repo.is_id_used(reused));
    EXPECT_EQ(repo.get_object(reused), "second");

    // the slot is recycled: the repository must not leak a live copy of the
    // removed object into iteration.
    int live = 0;
    for (auto const &value : repo)
    {
        EXPECT_EQ(value, "second");
        ++live;
    }
    EXPECT_EQ(live, 1);
}

TEST(value_repo, return_object_keeps_value_but_marks_slot_unused)
{
    value_repo<std::string> repo(1);

    auto id = repo.assign_object("kept");
    repo.return_object(id);

    EXPECT_FALSE(repo.is_id_used(id));

    int live = 0;
    for (auto const &value : repo)
    {
        (void)value;
        ++live;
    }
    EXPECT_EQ(live, 0);
}

TEST(value_repo, iterators_are_comparable_across_constness)
{
    value_repo<int> repo(2);
    repo.assign_object(1);

    EXPECT_FALSE(repo.begin() == repo.cend());
    EXPECT_TRUE(repo.begin() != repo.cend());
    EXPECT_TRUE(repo.cend() == repo.cend());

    auto cursor = repo.cbegin();
    ++cursor;
    EXPECT_TRUE(cursor == repo.cend());
}

TEST(value_repo, accessing_unused_slot_throws_in_debug_builds)
{
    value_repo<int> repo(1);
    auto id = repo.assign_object(1);
    repo.remove_object(id);

#ifndef NDEBUG
    EXPECT_THROW(repo.get_object(id), std::logic_error);
#else
    SUCCEED();
#endif
}

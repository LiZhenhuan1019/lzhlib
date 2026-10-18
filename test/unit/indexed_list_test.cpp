#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "lzhlib/list/indexed_list.hpp"

namespace
{
    using lzhlib::indexed_list;
    using lzhlib::null_index;
}

// The layout produced by push() is a circular list where the newest element
// becomes the head: 9 -> 8 -> ... -> 1 -> 0 -> (back to 9).
TEST(indexed_list, push_builds_circular_list_in_reverse_insertion_order)
{
    indexed_list<std::string> list(20);
    std::size_t head = null_index;

    for (std::size_t i = 0; i < 10; ++i)
    {
        std::size_t old_head = head;
        list.push(head, i);
        EXPECT_EQ(head, i);
        EXPECT_EQ(list.next(i), i == 0 ? 0 : old_head);
        EXPECT_EQ(list.previous(i), std::size_t{0});
    }
}

TEST(indexed_list, insert_at_tail_and_walk_both_directions)
{
    indexed_list<std::string> list(20);
    std::size_t head = null_index;

    for (std::size_t i = 0; i < 10; ++i)
    {
        list.push(head, i);
    }

    for (std::size_t i = 10; i < 20; ++i)
    {
        list.insert(head, null_index, i);
        EXPECT_EQ(list.previous(i), i == 10 ? 0 : i - 1);
        EXPECT_EQ(list.next(i), 9);
    }

    // 9 -> 8 -> ... -> 1 -> 0 -> 10 -> 11 ... -> 18 -> 19 -> (back to 9)
    // so prev(9) is the tail 19, while prev(k) is k + 1 for k in [0, 8].
    for (std::size_t i = 10; i > 0; --i)
    {
        EXPECT_EQ(list.next(i - 1), i == 1 ? 10 : i - 2);
        EXPECT_EQ(list.previous(i - 1), i == 10 ? 19 : i);
    }
}

TEST(indexed_list, pop_returns_head_and_updates_links)
{
    indexed_list<std::string> list(20);
    std::size_t head = null_index;

    for (std::size_t i = 0; i < 10; ++i)
    {
        list.push(head, i);
    }
    for (std::size_t i = 10; i < 20; ++i)
    {
        list.insert(head, null_index, i);
    }

    // popping walks 9, 8, ... 0 and leaves 10 as the new head
    for (std::size_t i = 10; i > 0; --i)
    {
        EXPECT_EQ(list.pop(head), i - 1);
    }
    EXPECT_EQ(head, 10);

    // the remaining ring is 10 -> 11 -> ... -> 19 -> 10
    std::vector<std::size_t> visited;
    std::size_t current = head;
    for (std::size_t i = 0; i < 10; ++i)
    {
        visited.push_back(current);
        current = list.next(current);
    }
    EXPECT_EQ(visited, (std::vector<std::size_t>{10, 11, 12, 13, 14, 15, 16, 17, 18, 19}));
    EXPECT_EQ(current, head);

    // removing every node, including the head, ends with an empty list
    for (std::size_t index : visited)
    {
        list.remove(head, index);
    }
    EXPECT_EQ(head, null_index);
    EXPECT_EQ(list.next(19), null_index);
    EXPECT_EQ(list.previous(10), null_index);
}

TEST(indexed_list, pop_last_element_resets_head_to_null_index)
{
    indexed_list<std::string> list(4);
    std::size_t head = null_index;

    list.push(head, 2);
    EXPECT_EQ(head, 2);
    EXPECT_EQ(list.pop(head), 2);
    EXPECT_EQ(head, null_index);
    EXPECT_EQ(list.next(2), null_index);
    EXPECT_EQ(list.previous(2), null_index);
}

TEST(indexed_list, form_list_links_a_contiguous_range)
{
    indexed_list<int> list(8);
    std::size_t head = list.form_list(2, 6);

    EXPECT_EQ(head, 2);
    EXPECT_EQ(list.previous(2), 5);
    EXPECT_EQ(list.next(5), 2);
    for (std::size_t i = 2; i < 5; ++i)
    {
        EXPECT_EQ(list.next(i), i + 1);
        EXPECT_EQ(list.previous(i + 1), i);
    }
}

TEST(indexed_list, form_list_rejects_invalid_ranges)
{
    indexed_list<int> list(4);
    EXPECT_THROW(list.form_list(2, 2), std::invalid_argument);  // empty range
    EXPECT_THROW(list.form_list(3, 2), std::invalid_argument);  // reversed range
    EXPECT_THROW(list.form_list(null_index, 1), std::invalid_argument);

    // The size is read through volatile on purpose: with a constant 4, GCC 12 at
    // -O3 proves that the range below is invalid and reports a bogus
    // -Warray-bounds for the accesses that the throw makes unreachable.
    volatile std::size_t const size = list.size();
    EXPECT_THROW(list.form_list(0, size + 1), std::invalid_argument);  // past the end
}

TEST(indexed_list, clear_unlinks_every_node)
{
    indexed_list<int> list(4);
    std::size_t head = list.form_list(0, 4);

    list.clear();

    for (std::size_t i = 0; i < 4; ++i)
    {
        EXPECT_EQ(list.next(i), null_index);
        EXPECT_EQ(list.previous(i), null_index);
    }
    // head is a caller owned index, clear() only resets node links
    EXPECT_EQ(head, 0);
}

TEST(indexed_list, null_index_is_propagated)
{
    indexed_list<int> list(4);
    EXPECT_EQ(list.next(null_index), null_index);
    EXPECT_EQ(list.previous(null_index), null_index);
}

// resize() appends value-initialized nodes; their links must stay null_index so
// that an unlinked node cannot be mistaken for the head of a list.
TEST(indexed_list, nodes_appended_by_resize_are_not_linked)
{
    indexed_list<int> list(2);
    list.resize(5);

    ASSERT_EQ(list.size(), 5u);
    for (std::size_t i = 2; i < 5; ++i)
    {
        EXPECT_EQ(list.next(i), null_index);
        EXPECT_EQ(list.previous(i), null_index);
    }
}

TEST(indexed_list, resize_keeps_the_links_of_the_existing_nodes)
{
    indexed_list<int> list(4);
    std::size_t head = list.form_list(0, 4);
    list.resize(8);

    EXPECT_EQ(head, 0);
    EXPECT_EQ(list.next(0), 1);
    EXPECT_EQ(list.previous(1), 0);
    EXPECT_EQ(list.previous(0), 3);
    EXPECT_EQ(list.next(3), 0);
}

TEST(indexed_list, at_is_checked_while_operator_subscript_is_not)
{
    indexed_list<int> list(2);
    list.at(1) = 7;
    EXPECT_EQ(list.at(1), 7);

    EXPECT_THROW(list.at(2), std::out_of_range);
    EXPECT_THROW(list.at(null_index), std::out_of_range);

    indexed_list<int> const &const_list = list;
    EXPECT_EQ(const_list.at(1), 7);
    EXPECT_THROW(const_list.at(2), std::out_of_range);
}

#include <cassert>

#define private public
#include "white_box_test.hpp"
#include "lzhlib/graph/graph.hpp"

using namespace std;
using namespace lzhlib;
using namespace lzhlib::detail;

void test_vertex()
{
    vertex<int> v{};
    edge_ref e1{edge_id(1), vertex_id()}, e2{edge_id(2), vertex_id()};
    v.add_associated_edge(e1);
    auto s = v.associated_edges();
    assert(s.size() == 1);
    assert(s.find(e1) != s.end() && s.find(e2) == s.end());

    v.add_associated_edge(e2);
    s = v.associated_edges();
    assert(s.size() == 2);
    assert(s.find(e1) != s.end() && s.find(e2) != s.end());

    v.remove_associated_edge(e1.edge());
    s = v.associated_edges();
    assert(s.size() == 1);
    assert(s.find(e1) == s.end() && s.find(e2) != s.end());

    v.remove_associated_edge(e2.edge());
    s = v.associated_edges();
    assert(s.empty());
    assert(s.find(e1) == s.end() && s.find(e2) == s.end());

    v.vertex_value() = 1;
    assert(v.vertex_value() == 1);
    v.vertex_value() = 2;
    assert(v.vertex_value() == 2);
}

void test_edge()
{
    edge<int> e;
    vertex_id v1{0}, v2{1};
    e.set_associated_vertices(v1, v2);
    assert(e.is_associated(v1) && e.is_associated(v2));

    edge<int>::pair_t p1 = e.associated_vertices();
    assert((p1.first == v1 && p1.second == v2) || (p1.first == v2 && p1.first == v1));

    e.detach();
    p1 = e.associated_vertices();
    assert(p1.first == invalid_vertex_id && p1.second == invalid_vertex_id);
    assert(!e.is_associated(v1) && !e.is_associated(v2));

    e.edge_value() = 1;
    assert(e.edge_value() == 1);
    e.edge_value() = 2;
    assert(e.edge_value() == 2);
}

void test_vertex_repository()
{
    dynamic_repository<vertex<int>> r0;
    auto i0 = r0.add_stock(std::in_place, 1);
    assert(r0.get_stock(i0).vertex_value() == 1);

    r0.remove_stock(i0);
    try
    {
        r0.get_stock(i0);
    }
    catch (dynamic_repository<vertex<int>>::attempt_to_use_unassigned_stock const&e)
    {
        assert(e.what() == std::string("Attempt to use unassigned stock whose id is ") +
                           std::to_string(i0.id()) + "!");
    }

}

void test_vertex_edge_and_repository()
{
    dynamic_repository<vertex<string>> rv;
    dynamic_repository<edge<string>> re;

    auto vs0 = rv.add_stock(std::in_place, "a");
    auto v0 = vertex_id(vs0);
    auto vs1 = rv.add_stock(std::in_place, "b");
    auto v1 = vertex_id(vs1);
    auto es0 = re.add_stock(std::in_place, "a and b");
    auto e0 =  edge_id(es0);

    rv.get_stock(vs0).add_associated_edge({e0, v1});
    rv.get_stock(vs1).add_associated_edge({e0, v0});
    re.get_stock(es0).set_associated_vertices(v0, v1);

    assert(re.get_stock(es0).is_associated(v0));
    assert(re.get_stock(es0).is_associated(v1));

    auto const &s0 = rv.get_stock(vs0).associated_edges();
    assert(s0.find({e0, v1}) != s0.end());

    auto const &s1 = rv.get_stock(vs1).associated_edges();
    assert(s1.find({e0, v0}) != s1.end());
}

void white_box_test::test()
{
    test_vertex();
    test_edge();
    test_vertex_repository();
    test_vertex_edge_and_repository();
}

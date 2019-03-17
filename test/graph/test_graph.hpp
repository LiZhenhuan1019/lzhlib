#ifndef LZHLIB_GRAPH_TEST_GRAPH_HPP
#define LZHLIB_GRAPH_TEST_GRAPH_HPP

#include <cassert>
#include "lzhlib/graph/graph.hpp"
#include "white_box_test.hpp"
namespace lzhlib::test
{
    namespace graph
    {
        using namespace std;
        using namespace lzhlib;
        using namespace lzhlib::detail;

        void test_undirected_graph()
        {
                undirected_graph<std::string, std::string> g;
                vertex_id v0 = g.add_vertex("v0");
                vertex_id v1 = g.add_vertex("v1");
                {
                    edge_id e0 = g.add_edge(v0, v1);
                    assert(g.value(e0) == "");
                    assert(g.get_edge(v0, v1) == e0);
                    g.value(e0) = "e0";
                    assert(g.associated(v0, e0));
                    assert(g.associated(v1, e0));
                    assert(g.adjacent(v0, v1));
                    assert(g.adjacent(v1, v0));
                    assert(g.get_edge(v0, v1) == e0);
                    assert(g.get_edge(v1, v0) == e0);
                    assert(g.value(v0) == "v0");
                    assert(const_cast<decltype(g)const &>(g).value(v0) == "v0");
                    assert(g.value(v1) == "v1");
                    assert(const_cast<decltype(g)const &>(g).value(v1) == "v1");
                    assert(g.value(e0) == "e0");
                    assert(const_cast<decltype(g)const &>(g).value(e0) == "e0");
                    {
                        auto vertex = g.neighbors(v0);
                        assert(vertex.size() == 1);
                        assert(vertex[0] == v1);
                    }
                    {
                        auto vertex = g.neighbors(v1);
                        assert(vertex.size() == 1);
                        assert(vertex[0] == v0);
                    }
                    {
                        auto edge_refs = g.associated_edges(v0);
                        assert(edge_refs.size() == 1);
                        assert((*edge_refs.begin()).edge() == e0);
                        assert((*edge_refs.begin()).opposite_vertex() == v1);
                    }
                    {
                        auto edge_refs = g.associated_edges(v1);
                        assert(edge_refs.size() == 1);
                        assert((*edge_refs.begin()).edge() == e0);
                        assert((*edge_refs.begin()).opposite_vertex() == v0);
                    }
                    auto vertices = g.associated_vertices(e0);
                    assert(vertices.first == v0 && vertices.second == v1);
                    set<vertex_id> vertices0 = {vertices.first, vertices.second};
                    set<vertex_id> vertices1;
                    for (auto v = g.first_vertex(); v != g.end_vertex(); v = g.next_vertex(v))
                        vertices1.insert(v);
                    assert(vertices0 == vertices1);

                    g.remove_edge(e0);
                    assert(!g.associated(v0, e0));
                    assert(!g.associated(v1, e0));
                    assert(!g.adjacent(v0, v1));
                    assert(!g.adjacent(v1, v0));
                }
                {
                    edge_id e1 = g.add_edge(v0, v1, "e1");
                    assert(g.value(e1) == "e1");
                    assert(g.associated(v0, e1));
                    assert(g.associated(v1, e1));
                    assert(g.adjacent(v0, v1));
                    assert(g.adjacent(v1, v0));
                    assert(g.get_edge(v0, v1) == e1);
                    assert(g.get_edge(v1, v0) == e1);

                    g.remove_edge(v0, v1);
                    assert(!g.associated(v0, e1));
                    assert(!g.associated(v1, e1));
                    assert(!g.adjacent(v0, v1));
                    assert(!g.adjacent(v1, v0));
                }
                g.remove_vertex(v0);
                g.remove_vertex(v1);
            }


        void test_directed_graph()
        {
            directed_graph<std::string, std::string> g;
            vertex_id v0 = g.add_vertex("v0");
            vertex_id v1 = g.add_vertex("v1");
            {
                edge_id e0 = g.add_edge(v0, v1);
                assert(g.value(e0) == "");
                assert(g.get_edge(v0, v1) == e0);
                assert(g.associated(v0, e0));
                assert(!g.associated(v1, e0));
                assert(g.adjacent(v0, v1));
                assert(!g.adjacent(v1, v0));
                assert(g.get_edge(v0, v1) == e0);
                //assert(g.get_edge(v1, v0) != e0);
                assert(g.value(v0) == "v0");
                assert(const_cast<decltype(g)const &>(g).value(v0) == "v0");
                assert(g.value(v1) == "v1");
                assert(const_cast<decltype(g)const &>(g).value(v1) == "v1");
                g.value(e0) = "e0";
                assert(g.value(e0) == "e0");
                assert(const_cast<decltype(g)const &>(g).value(e0) == "e0");
                {
                    auto vertices = g.neighbors(v0);
                    assert(vertices.size() == 1);
                    assert(vertices[0] == v1);
                }
                {
                    auto vertices = g.neighbors(v1);
                    assert(vertices.empty());
                }

                {
                    auto edge_refs = g.associated_edges(v0);
                    assert(edge_refs.size() == 1);
                    assert((*edge_refs.begin()).edge() == e0);
                    assert((*edge_refs.begin()).opposite_vertex() == v1);
                }
                {
                    auto edge_refs = g.associated_edges(v1);
                    assert(edge_refs.empty());
                }

                {
                    auto vertices = g.associated_vertices(e0);
                    assert(vertices.first == v0 && vertices.second == v1);
                    set<vertex_id> vertices0 = {vertices.first, vertices.second};
                    set<vertex_id> vertices1;
                    for (auto v = g.first_vertex(); v != g.end_vertex(); v = g.next_vertex(v))
                        vertices1.insert(v);
                    assert(vertices0 == vertices1);
                }

                g.remove_edge(e0);
                assert(!g.associated(v0, e0));
                assert(!g.associated(v1, e0));
                assert(!g.adjacent(v0, v1));
                assert(!g.adjacent(v1, v0));
                {
                    edge_id e1 = g.add_edge(v0, v1, "e1");
                    assert(g.value(e1) == "e1");
                    assert(g.adjacent(v0, v1));
                    assert(!g.adjacent(v1, v0));
                    assert(g.get_edge(v0, v1) == e1);
                    //assert(g.get_edge(v1, v0) != e1);

                    g.remove_edge(v0, v1);
                    assert(!g.associated(v0, e0));
                    assert(!g.associated(v1, e0));
                    assert(!g.adjacent(v0, v1));
                    assert(!g.adjacent(v1, v0));
                }
            }
            g.remove_vertex(v0);
            g.remove_vertex(v1);
        }
        void test()
        {
            white_box_test().test();
            test_undirected_graph();
            test_directed_graph();
        }
    }
}
#endif //LZHLIB_GRAPH_TEST_GRAPH_HPP

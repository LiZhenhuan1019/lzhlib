#include <cassert>
#include <lzhlib/graph/undirected_graph.hpp>

int main()
{
    lzhlib::undirected_graph<std::string> g;
    auto v1 = g.add_vertex("first vertex");
    auto v2 = g.add_vertex("second vertex");
    auto e1 = g.add_edge(v1, v2);
    auto edges = g.associated_edges(v1);
    auto iter = edges.find(e1);
    assert(iter->edge() == e1);
    assert(iter->opposite_vertex() == v2);
}
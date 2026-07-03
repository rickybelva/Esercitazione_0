#include <cstdlib>
#include <iostream>
#include <set>
#include <vector>
#include "unidirected_edge.h"
#include "unidirected_graph.h"


bool test_edge_ordering()
{
    unidirected_edge e1(5, 2);
    return e1.from() == 2 && e1.to() == 5;
}
bool test_edge_equality()
{
    unidirected_edge e1(1, 4);
    unidirected_edge e2(4, 1);
    unidirected_edge e3(1, 5);
    return (e1 == e2) && !(e1 == e3);
}
bool test_add_edge_and_nodes()
{
    unidirected_graph g;
    g.add_edge(1, 2);
    g.add_edge(2, 3);
    std::set<int> expected_nodes = {1, 2, 3};
    std::set<unidirected_edge> expected_edges = {
        unidirected_edge(1, 2),
        unidirected_edge(2, 3)
    };
    return g.all_nodes() == expected_nodes && g.all_edges() == expected_edges;
}
bool test_neighbours()
{
    unidirected_graph g;
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 4);
    std::set<int> expected_1 = {2, 3};
    std::set<int> expected_2 = {1, 4};
    std::set<int> expected_5 = {};
    return g.neighbours(1) == expected_1
        && g.neighbours(2) == expected_2
        && g.neighbours(5) == expected_5;
}
bool test_no_duplicate_edges()
{
    unidirected_graph g;
    g.add_edge(1, 2);
    g.add_edge(2, 1);
    g.add_edge(unidirected_edge(1, 2));
    return g.all_edges().size() == 1;
}
bool test_edge_number_and_edge_at()
{
    unidirected_graph g;
    g.add_edge(10, 20);
    g.add_edge(20, 30);
    g.add_edge(5, 7);
    bool cond1 = (g.edge_number(unidirected_edge(10, 20)) == 0);
    bool cond2 = (g.edge_number(unidirected_edge(20, 30)) == 1);
    bool cond3 = (g.edge_number(unidirected_edge(5, 7)) == 2);
    bool cond4 = (g.edge_at(0) == unidirected_edge(10, 20));
    bool cond5 = (g.edge_at(1) == unidirected_edge(20, 30));
    bool cond6 = (g.edge_at(2) == unidirected_edge(5, 7));
    return cond1 && cond2 && cond3 && cond4 && cond5 && cond6;
}
bool test_graph_difference()
{
    unidirected_graph g1;
    g1.add_edge(1, 2);
    g1.add_edge(2, 3);
    g1.add_edge(3, 4);
    unidirected_graph g2;
    g2.add_edge(2, 3);
    unidirected_graph diff = g1 - g2;
    std::set<unidirected_edge> expected = {
        unidirected_edge(1, 2),
        unidirected_edge(3, 4)
    };
    return diff.all_edges() == expected;
}
bool test_copy_constructor()
{
    unidirected_graph g1;
    g1.add_edge(1, 2);
    g1.add_edge(2, 5);
    unidirected_graph g2(g1);
    return g2.all_edges() == g1.all_edges()
        && g2.all_nodes() == g1.all_nodes()
        && g2.neighbours(2) == g1.neighbours(2);
}
bool test_exceptions()
{
    unidirected_graph g;
    g.add_edge(1, 2);
    bool edge_at_exception = false;
    bool edge_number_exception = false;
    try {
        g.edge_at(5);
    } catch (const std::out_of_range&) {
        edge_at_exception = true;
    }
    try {
        g.edge_number(unidirected_edge(7, 8));
    } catch (const std::out_of_range&) {
        edge_number_exception = true;
    }
    return edge_at_exception && edge_number_exception;
}
int main()
{
    if (!test_edge_ordering()) {
        std::cerr << "Errore nel test_edge_ordering\n";
        return EXIT_FAILURE;
    }
    if (!test_edge_equality()) {
        std::cerr << "Errore nel test_edge_equality\n";
        return EXIT_FAILURE;
    }
    if (!test_add_edge_and_nodes()) {
        std::cerr << "Errore nel test_add_edge_and_nodes\n";
        return EXIT_FAILURE;
    }
    if (!test_neighbours()) {
        std::cerr << "Errore nel test_neighbours\n";
        return EXIT_FAILURE;
    }
    if (!test_no_duplicate_edges()) {
        std::cerr << "Errore nel test_no_duplicate_edges\n";
        return EXIT_FAILURE;
    }
    if (!test_edge_number_and_edge_at()) {
        std::cerr << "Errore nel test_edge_number_and_edge_at\n";
        return EXIT_FAILURE;
    }
    if (!test_graph_difference()) {
        std::cerr << "Errore nel test_graph_difference\n";
        return EXIT_FAILURE;
    }
    if (!test_copy_constructor()) {
        std::cerr << "Errore nel test_copy_constructor\n";
        return EXIT_FAILURE;
    }
    if (!test_exceptions()) {
        std::cerr << "Errore nel test_exceptions\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
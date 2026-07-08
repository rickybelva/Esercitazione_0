#include <iostream>
#include "unidirected_edge.h"
#include "unidirected_graph.h"
#include "adapters.h"
#include "graph_visit.h"
#include "dfs_recursive.h"
#include "dijkstra.h"
int main()
{
    unidirected_graph G;
    G.add_edge(1,2);
    G.add_edge(2,3);
    G.add_edge(3,4);
    G.add_edge(4,5);
    G.add_edge(2,5);
    // DFS ricorsiva
    auto dfsG = dfs_recursive(G, 1);
    // DFS iterativa (LIFO)
    lifo s;
    auto dfsIter = graph_visit(G, 1, s);
    // BFS (FIFO)
    fifo q;
    auto bfsG = graph_visit(G, 1, q);
    // Dijkstra con pesi unitari
    auto weight = [&](int u, int v) -> double { (void)u; (void)v; return 1.0; };
    auto dj = dijkstra(G, 1, weight);
    std::cout << "Nodi BFS: ";
    for (int v : bfsG.all_nodes()) std::cout << v << " ";
    std::cout << "\n";
    std::cout << "Archi BFS:\n";
    for (auto e : bfsG.all_edges()) std::cout << e << "\n";
    std::cout << "Dist Dijkstra (1->v):\n";
    for (int v : G.all_nodes()) std::cout << v << ": " << dj.dist[v] << "\n";
    return 0;
}
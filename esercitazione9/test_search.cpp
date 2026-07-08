#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "unidirected_edge.h"
#include "unidirected_graph.h"
#include "adapters.h"
#include "graph_visit.h"
#include "dfs_recursive.h"
#include "dijkstra.h"
static unidirected_graph make_sample_graph()
{
    unidirected_graph G;
    // Grafo con due “rami” e qualche collegamento incrociato
    G.add_edge(1,2);
    G.add_edge(2,3);
    G.add_edge(3,4);
    G.add_edge(2,5);
    G.add_edge(5,6);
    G.add_edge(3,6);
    return G;
}
int main()
{
    auto G = make_sample_graph();
    // BFS via fifo
    fifo q;
    auto bfsT = graph_visit(G, 1, q);
    // DFS iterativa via lifo
    lifo s;
    auto dfsT = graph_visit(G, 1, s);
    // DFS ricorsiva
    auto dfsRecT = dfs_recursive(G, 1);
    // Verifiche minime: sorgente deve comparire nei nodi degli alberi
    if (!bfsT.all_nodes().count(1)) { std::cerr << "BFS tree missing source\n"; return EXIT_FAILURE; }
    if (!dfsT.all_nodes().count(1)) { std::cerr << "DFS tree missing source\n"; return EXIT_FAILURE; }
    if (!dfsRecT.all_nodes().count(1)) { std::cerr << "DFS-rec tree missing source\n"; return EXIT_FAILURE; }
    // Dijkstra con pesi semplici: peso(u,v) = 1 (equivale alla distanza in numero di archi)
    auto weight = [&](int u, int v) -> double {
        (void)u; (void)v; return 1.0;
    };
    auto dj = dijkstra(G, 1, weight);
    // dist della sorgente deve essere 0
    if (dj.dist[1] != 0.0) { std::cerr << "Dijkstra dist[source]!=0\n"; return EXIT_FAILURE; }
    // Tutti i nodi raggiungibili da 1 devono avere dist finite (nel nostro grafo lo sono tutti)
    for (int v : G.all_nodes()) {
        if (dj.dist[v] == std::numeric_limits<double>::infinity()) {
            std::cerr << "Dijkstra node unreachable: " << v << "\n";
            return EXIT_FAILURE;
        }
    }
    // L'albero di Dijkstra deve avere |V|-1 archi se il grafo è connesso
    if (dj.tree.all_edges().size() != G.all_nodes().size() - 1) {
        std::cerr << "Dijkstra tree size mismatch\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
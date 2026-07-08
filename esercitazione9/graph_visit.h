#pragma once
#include <map>
#include <set>
#include <utility>
#include "unidirected_graph.h"
// Contenitore richiesto: deve avere put(int), get()->int, empty()->bool
template<typename Container>
unidirected_graph graph_visit(const unidirected_graph& G, int source, Container& container)
{
    unidirected_graph tree;
    if (!G.all_nodes().count(source)) {
        return tree; // sorgente non presente: albero vuoto
    }
    std::map<int, bool> reached;
    for (int v : G.all_nodes()) {
        reached[v] = false;
    }
    container.put(source);
    while (!container.empty()) {
        int u = container.get();
        if (reached[u]) continue;
        reached[u] = true;
        for (int w : G.neighbours(u)) {
            if (!reached[w]) {
                // albero: collega u -> w
                tree.add_edge(u, w);
                container.put(w);
            }
        }
    }
    return tree;
}
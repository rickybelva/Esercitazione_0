#pragma once
#include <map>
#include <set>
#include "unidirected_graph.h"
namespace detail {
    inline void dfs_rec_impl(const unidirected_graph& G, int u,
                             std::map<int,bool>& vis, unidirected_graph& tree)
    {
        vis[u] = true;
        for (int w : G.neighbours(u)) {
            if (!vis[w]) {
                tree.add_edge(u, w);
                dfs_rec_impl(G, w, vis, tree);
            }
        }
    }
}
inline unidirected_graph dfs_recursive(const unidirected_graph& G, int source)
{
    unidirected_graph tree;
    if (!G.all_nodes().count(source)) return tree;
    std::map<int,bool> vis;
    for (int v : G.all_nodes()) vis[v] = false;
    detail::dfs_rec_impl(G, source, vis, tree);
    return tree;
}
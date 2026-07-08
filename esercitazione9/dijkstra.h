#pragma once
#include <queue>
#include <limits>
#include <map>
#include <set>
#include <utility>
#include "unidirected_graph.h"
// Peso: callable con firma double(int u, int v)
// Deve restituire il peso dell'arco (u,v). Gli archi assenti non verranno mai richiesti.
template<typename WeightFunc>
struct dijkstra_result {
    std::map<int,double> dist;
    std::map<int,int> pred;
    unidirected_graph tree; // albero dei cammini minimi
};
template<typename WeightFunc>
dijkstra_result<WeightFunc>
dijkstra(const unidirected_graph& G, int source, WeightFunc weight)
{
    using P = std::pair<double,int>; // (dist, node)
    const double INF = std::numeric_limits<double>::infinity();
    // Insieme dei nodi del grafo
    std::set<int> nodes = G.all_nodes();
    std::map<int,double> dist;
    std::map<int,int> pred;
    for (int v : nodes) {
        dist[v] = INF;
        pred[v] = -1;
    }
    if (!nodes.count(source)) {
        // grafo senza sorgente: risultato vuoto
        return {dist, pred, unidirected_graph{}};
    }
    dist[source] = 0.0;
    pred[source] = source;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.emplace(0.0, source);
    // Per costruire l’albero: quando rilassiamo definitivamente un nodo w con pred[w]=u
    // aggiungiamo l’arco (u,w) all’albero.
    unidirected_graph tree;
    std::set<int> finalized; // opzionale: per evitare rielaborazioni e marcare estrazioni definitive
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue; // entry obsoleta
        // Marca come finalizzato (opzionale)
        finalized.insert(u);
        // rilassamento sugli adiacenti
        for (int w : G.neighbours(u)) {
            double alt = dist[u] + weight(u, w);
            if (alt < dist[w]) {
                dist[w] = alt;
                pred[w] = u;
                pq.emplace(dist[w], w);
            }
        }
    }
    // Costruzione dell’albero dai predecessori
    for (auto& [v, p] : pred) {
        if (v == source || p == -1) continue;
        if (p == v) continue;
        if (dist[v] < INF) {
            tree.add_edge(p, v);
        }
    }
    return {dist, pred, tree};
}
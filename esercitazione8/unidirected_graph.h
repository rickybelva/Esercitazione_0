#pragma once
#include <map>
#include <set>
#include <stdexcept>
#include <vector>
#include "unidirected_edge.h"

//In questa implementazione un nodo viene creato implicitamente quando compare in almeno un arco.
//Questa scelta e' coerente con la traccia, che non richiede esplicitamente un metodo add_node().

class unidirected_graph
{
private:
    std::set<unidirected_edge> m_edges;
    std::map<int, std::set<int>> m_adjacency;
    std::vector<unidirected_edge> m_edge_order;
public:
    unidirected_graph() = default;
    unidirected_graph(const unidirected_graph& other) = default;
    std::set<int> neighbours(int node) const
    {
        auto it = m_adjacency.find(node);
        if (it == m_adjacency.end()) {
            return {};
        }
        return it->second;
    }
    void add_edge(const unidirected_edge& edge)
    {
        if (m_edges.find(edge) != m_edges.end()) {
            return;
        }
        m_edges.insert(edge);
        m_edge_order.push_back(edge);
        int a = edge.from();
        int b = edge.to();
        m_adjacency[a].insert(b);
        m_adjacency[b].insert(a);
    }
    void add_edge(int a, int b)
    {
        add_edge(unidirected_edge(a, b));
    }
    std::set<unidirected_edge> all_edges() const
    {
        return m_edges;
    }
    std::set<int> all_nodes() const
    {
        std::set<int> nodes;
        for (const auto& [node, _] : m_adjacency) {
            nodes.insert(node);
        }
        return nodes;
    }
    std::size_t edge_number(const unidirected_edge& edge) const
    {
        for (std::size_t i = 0; i < m_edge_order.size(); ++i) {
            if (m_edge_order[i] == edge) {
                return i;
            }
        }
        throw std::out_of_range("edge not found");
    }
    unidirected_edge edge_at(std::size_t index) const
    {
        if (index >= m_edge_order.size()) {
            throw std::out_of_range("invalid edge index");
        }
        return m_edge_order[index];
    }
    unidirected_graph operator-(const unidirected_graph& other) const
    {
        unidirected_graph result;
        for (const auto& edge : m_edge_order) {
            if (other.m_edges.find(edge) == other.m_edges.end()) {
                result.add_edge(edge);
            }
        }
        return result;
    }
};
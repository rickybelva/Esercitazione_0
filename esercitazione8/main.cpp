#include <iostream>
#include "unidirected_edge.h"
#include "unidirected_graph.h"

int main()
{
    unidirected_graph g;
    g.add_edge(3, 1);
    g.add_edge(2, 5);
    g.add_edge(1, 2);
    std::cout << "Nodi del grafo:\n";
    for (int node : g.all_nodes()) {
        std::cout << node << " ";
    }
    std::cout << "\n\n";
    std::cout << "Archi del grafo:\n";
    for (const auto& edge : g.all_edges()) {
        std::cout << edge << "\n";
    }
    std::cout << "\n";
    std::cout << "Vicini del nodo 1:\n";
    for (int neigh : g.neighbours(1)) {
        std::cout << neigh << " ";
    }
    std::cout << "\n";
	std::cout << "Numero dell'arco (1,2):\n";
	std::cout << g.edge_number(unidirected_edge(1,2)) << "\n";
	std::cout << "\n";
	std::cout << "L'arco numero 1 è:\n";
	std::cout << g.edge_at(1) << "\n";
	
	unidirected_graph g1;
	g1.add_edge(3, 1);
    g1.add_edge(2, 6);
    g1.add_edge(1, 6);
	std::cout << "\n";
	std::cout << "I nodi del grafo differenza tra g e g1 sono:\n";
	unidirected_graph d=g-g1;
	for (int node : d.all_nodes()) {
        std::cout << node << " ";
    }
	std::cout << "\n\n";
	
	
    return 0;
}
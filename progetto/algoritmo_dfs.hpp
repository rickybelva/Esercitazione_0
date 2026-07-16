#pragma once
#include <vector>
#include <map>
#include <string>
#include <concepts>
#include <set>
#include <optional>
#include "grafo.hpp"
#include "visite_grafi.hpp"

// Algoritmo cicli minimi basato su DFS

// funzione ricorsiva per trovare il percorso tra due nodi all'interno dell'albero DFS
template<typename I> requires std::integral<I>
bool trova_percorso(const unidirected_graph<I>& albero_dfs, I u, I v, std::vector<I>& percorso, std::set<I>& visited) {
	
	visited.insert(u);
	percorso.push_back(u);
	
	if (u == v) {
		return true; //se il nodo corrente coincide con quello di arrivo il percorso e' stato trovato!
	}
	
	for (I vicino : albero_dfs.neighbours(u)) { //itero su tutti i vicini di u nell'albero dfs
		if (!visited.contains(vicino)) { // si evitano i nodi gia' visitati (cosi' non si torna indietro)
			if (trova_percorso(albero_dfs, vicino, v, percorso, visited)) {
				return true; // chiamata ricorsiva: provo a cercare il percorso partendo dal vicino
			}
		}
	}
	
	percorso.pop_back(); // questa istruzione viene eseguita quando da u non e' stato possibile raggiungere v (backtracking)
	// u viene rimosso dal percorso, quindi si cerchera' un'altra strada
	// u invece viene lasciato all'interno di visited per memorizzare che è stato visitato
	
	return false;
}

// funzione che trova i cicli fondamentali a partire dall'albero DFS e dal coalbero
template<typename I> requires std::integral<I>
std::vector<std::vector<I>> cicli_fondamentali_dfs(const unidirected_graph<I>& albero_dfs, const unidirected_graph<I>& coalbero){

	// numero di archi del coalbero
	int k = coalbero.all_edges().size();
	//vettore dei cicli: ogni ciclo e' a sua volta rappresentato come un vettore
	std::vector<std::vector<I>> cicli;
	//il numero di archi del coalbero coincide con il numero di cicli perche' ogni arco del coalbero chiude un ciclo dell'albero DFS
	cicli.reserve(k);
	
	for (const auto& edge : coalbero.all_edges()) { //itero sugli archi del coalbero
		I u = edge.from();
		I v = edge.to();

		std::vector<I> percorso; 
		std::set<I> visited;
		
		if (trova_percorso(albero_dfs, u, v, percorso, visited)) {
			//trova_percorso restituisce true se esiste il percorso da u a v
			//inoltre restituisce il percorso fatto da u a v

			// Aggiungo il nodo u alla fine per chiudere il ciclo
			percorso.push_back(u);
			//aggiungo il percorso alla lista dei cicli
			cicli.push_back(percorso);
		}
		
	}
	
	return cicli; 
}

// algoritmo DFS che utilizza un contenitore LIFO per ottenere l'albero di ricoprimento DFS e il relativo coalbero
std::vector<std::vector<int>> algoritmo_dfs(const std::map<std::string, valori_numerici>& circuito) {
	unidirected_graph<int> grafo_circuito;
	for(const auto& [_, nodi] : circuito){
		//inserisco tutti gli archi del circuito in un grafo che modellizza il circuito
		grafo_circuito.add_edge(nodi.n1,nodi.n2);
	}
		
	lifo<int> stack;
	unidirected_graph<int> albero_dfs = visite_grafi(grafo_circuito, grafo_circuito.all_nodes()[0], stack); 
	unidirected_graph<int> coalbero = grafo_circuito - albero_dfs; //coalbero ottenuto come differenza del grafo circuito - albero_dfs
	
	// le maglie del circuito corrispondono ai cicli fondamentali che trovo utilizzando l'albero DFS e il coalbero
	std::vector<std::vector<int>> maglie = cicli_fondamentali_dfs(albero_dfs, coalbero);
	
	// restituisco le maglie (vettore di vettori)
	return maglie;
}	
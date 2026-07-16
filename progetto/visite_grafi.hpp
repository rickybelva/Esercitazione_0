#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <concepts>
#include <algorithm>
#include <set>
#include <optional>
#include <iterator> 
#include <list>
#include "grafo.hpp"

// Contenitori FIFO e LIFO

template<typename T>
class fifo {
    std::list<T> coda; //modellizzo un contenitore FIFO (in pratica una coda) con una lista
public:
	fifo() = default;
	
	//metodo put che prende un elemento e lo inserisce alla fine della lista
	void put(const T& x){
		coda.push_back(x);
	}
	
	//metodo get che restituisce il primo elemento della lista (First In First Out)
	std::optional<T> get(){
		if(coda.empty()){
			return std::nullopt;
		} else {
			T f = coda.front();
			coda.pop_front();
			return f;
		}
	}
	
	//metodo empty che controlla se la coda è vuota
	bool empty() const {
		return coda.empty();
	}
};

template<typename T>
class lifo {
    std::list<T> stack; //modellizzo un contenitore LIFO (in pratica uno stack) con una lista
public:
	lifo() = default;
	
	//metodo put che inserisce un elemento all'inizio della lista
	void put(const T& x){
		stack.push_front(x);
	}
	
	//metodo get che restituisce il primo elemento della lista (Last In First Out)
	std::optional<T> get(){
		if(stack.empty()){
			return std::nullopt;
		} else {
			T f = stack.front();
			stack.pop_front();
			return f;
		}
	}
	
	//metodo empty che controlla se lo stack è vuoto
	bool empty() const {
		return stack.empty();
	}
};

// Visite grafi (DFS e BFS) in base all'indicazione del contenitore da utilizzare

template<typename I> requires std::integral<I>
unidirected_graph<I> visite_grafi (const unidirected_graph<I>& grafo, const I& nodo_partenza, auto& contenitore){
	std::set<I> reached;
	unidirected_graph<I> albero_ric;
	albero_ric.add_node(nodo_partenza); // mi assicuro che il nodo di partenza venga inserito nell'albero di ricoprimento 
	
	contenitore.put(nodo_partenza);
	reached.insert(nodo_partenza);
	while(!contenitore.empty()){
		auto u_p = contenitore.get();
		if (u_p!=std::nullopt) {
			auto u = *u_p;
			for(const I& vicino : grafo.neighbours(u)) { //scorro tutti i vicini di u
				if(!reached.contains(vicino)) {
					reached.insert(vicino);
					contenitore.put(vicino);
					albero_ric.add_edge(u,vicino);
				}
    		}
		}
	}
	return albero_ric;
}



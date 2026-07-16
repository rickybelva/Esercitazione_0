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

//crea una classe unidirected_edge che modella gli archi di un grado non orientato
template<typename I> requires std::integral<I>
class unidirected_edge {
    I start;
    I end;
    
public:
	//Non voglio un costruttore di default. I nodi dell'arco devono sempre essere specificati
    unidirected_edge() = delete;
    
	//Costruttore user-defined che ordina i nodi in modo che l'arco sia sempre percorso dal nodo di indice minore a quello maggiore
    unidirected_edge(I a, I b) {
        if (a <= b) {
            start = a;
            end = b;
        } else {
            start = b;
            end = a;
        }
    }

	//Metodi from() e to() che restituiscono il nodo di partenza e quello di arrivo
    I from() const { 
		return start; 
	}
    I to() const {
		return end;
	}
    
	//Operatore < per ordinare gli archi
    bool operator<(const unidirected_edge& other) const {
        if (start < other.start) {
            return true;
        }
		if (start > other.start) {
            return false;
        }
        return end < other.end;
    }
    
	//Operatore == per verificare se due archi coincidono
    bool operator==(const unidirected_edge& other) const {
        return start == other.start && end == other.end;
    }    
};

//Operatore << per stampare gli archi
template<typename I> requires std::integral<I>
std::ostream& operator<<(std::ostream& os, const unidirected_edge<I>& edge) {
    os << "(" << edge.from() << ", " << edge.to() << ")";
    return os;
}

template<typename I> requires std::integral<I>
class unidirected_graph {
	//Lista di adiacenza
    std::map<I, std::set<I>> lista_ad;

	//Vettore degli archi numerati
    std::vector<unidirected_edge<I>> vettore_archi;

public:
    unidirected_graph() = default;
    
    void add_node(I n) {
	    if(lista_ad.contains(n)){
		    std::cerr << "Errore: il nodo "<< n << " era gia' presente nel grafo.\n";
	    } else{
        	lista_ad[n]; //il nodo viene aggiunto alla lista e il set di adiacenza sara' inzialmente vuoto
        }
    }

    void add_edge(const unidirected_edge<I>& edge) {
	    I f = edge.from();
		I t = edge.to();
		
		//se i nodi dell'arco non sono gia' presenti, li inserisco
	    if(!lista_ad.contains(f)){
		    add_node(f);
	    }
	    if(!lista_ad.contains(t)){
		    add_node(t);
	    }
		
		//controllo se l'arco era gia' presente nel grafo
		if(lista_ad[f].contains(t)){
			std::cerr << "Errore: l'arco (" << f << ", " << t << ") era gia' presente nel grafo.\n";
		} else{
			//aggiungo i nodi dell'arco con il metodo insert
			lista_ad[f].insert(t);
			lista_ad[t].insert(f);
			
			// lower_bound trova il punto esatto in cui inserire per mantenere il vettore ordinato e inserisce la posizione in un iteratore
			auto it = std::lower_bound(vettore_archi.begin(), vettore_archi.end(), edge);
			//inserisco l'arco nella posizione corrispondente all'iteratore it
			vettore_archi.insert(it, edge);		
		}
    }
	

    void remove_edge(const unidirected_edge<I>& edge) {
	    I f = edge.from();
		I t = edge.to();
		
		//controllo che l'arco che voglio togliere sia effettivamente presente nel grafo
	    if(!lista_ad.contains(f) || !lista_ad.contains(t) || !lista_ad[f].contains(t)){
		    std::cerr << "Errore: l'arco (" << f << ", " << t << ") non esiste nel grafo.\n";
	    } else {
		    //elimino i nodi dell'arco con il metodo erase
			lista_ad[f].erase(t);
			lista_ad[t].erase(f);
			
			// cerco la posizione dell'arco nel vettore degli archi
			auto it = std::lower_bound(vettore_archi.begin(), vettore_archi.end(), edge);    
			
			// controllo che l'iteratore it punti effettivamente all'arco desiderato, dunque lo cancello dal vettore degli archi
			if (it != vettore_archi.end() && *it == edge) {
            	vettore_archi.erase(it);
        	}
		}
	}

	//metodo add_edge che riceve in input due nodi invece dell'arco
    void add_edge(I f, I t) {
        unidirected_edge<I> new_edge(f, t);
        add_edge(new_edge);
    }

    
    std::set<I> neighbours(I node) const {
	    if(lista_ad.contains(node)){
		    //restituisco il set dei vicini del nodo
	    	return lista_ad.at(node);
	    } else {
		    std::cerr << "Errore: il nodo "<< node << " non e' presente nel grafo.\n";
		    //restituisco un set vuoto
		    return {};
	    }
    }
    
	//metodo all_nodes per ottenere tutti i nodi del grafo ordinati
    std::vector<I> all_nodes() const{
	    std::vector<I> nodes(lista_ad.size()); 
	    int i = 0;
        for (const auto& [node, _] : lista_ad) {
            nodes[i] = node;
            i++;
        }
        
        return nodes; //il vettore dei nodi e' ordinato perche' i nodi sono le chiavi della lista di adiacenza (che e' una std::map)
    }
    
    std::vector<unidirected_edge<I>> all_edges() const {
		return vettore_archi; //funzione che restituisce il vettore degli archi 
	}
	
	//funzione che trova la posizione di un arco nel vettore degli archi
	//se l'arco esiste, restituisce il suo indice, altrimenti restituisce std::nullopt
	//size_t e' il tipo degli indici dei contenitori della STL (interi senza segno)
	std::optional<size_t> edge_number(const unidirected_edge<I>& edge) const {
		
		auto it = std::lower_bound(vettore_archi.begin(), vettore_archi.end(), edge);
	
		if (it != vettore_archi.end() && *it == edge) {
			//il metodo distance restituisce la distanza tra begin e it, dunque restituisce la posizione dell'arco nel vettore
			return std::distance(vettore_archi.begin(), it);
		}
		return std::nullopt;
	}
	
	//funzione che, dato un indice, restituisce l'arco corrispondente del grafo
	std::optional<unidirected_edge<I>> edge_at(size_t indice) const {
		if (indice < vettore_archi.size()) {
			return vettore_archi[indice];
		}
		return std::nullopt;
	}
    
	//overload dell'operatore '=' per copiare un grafo all'interno di un altro
    unidirected_graph& operator=(const unidirected_graph& other) {
        if (this != &other) { //controllo se sto copiando un grafo diverso dal grafo corrente
            lista_ad = other.lista_ad;
            vettore_archi = other.vettore_archi;
        }
        return *this;
    }
    
	//overload dell'operatore '-' per fare la differenza tra due grafi (utile per ottenere il coalbero)
    unidirected_graph operator-(const unidirected_graph& other) const {
	    unidirected_graph differenza = *this;
	    for (const auto& edge : other.all_edges()) { //itero su tutti gli archi presenti nell'altro grafo
            differenza.remove_edge(edge); //rimuovo dal primo grafo gli archi che sono presenti anche nel secondo grafo
        }
        return differenza;
    }    
};
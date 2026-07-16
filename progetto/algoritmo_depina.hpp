#pragma once
#include <vector>
#include <map>
#include <string>
#include <concepts>
#include <set>
#include <optional>
#include <algorithm>
#include "grafo.hpp"
#include "visite_grafi.hpp"

// Algoritmo di De Pina per trovare cicli minimi

// L'algoritmo lavora su vettori booleani di lunghezza m = |E| (numero di archi),
// indicizzati secondo l'ordine lessicografico degli archi. Nel progetto questo
// ordine e' proprio quello del vettore restituito da all_edges(), e edge_number()
// fornisce l'indice lessicografico di un arco: sono esattamente le due primitive
// di cui De Pina ha bisogno.


// Prodotto scalare mod 2 tra due vettori booleani
// <S, P> = somma (S_i * P_i) mod 2.
// Vale 1 se S e P hanno un numero DISPARI di 1 in posizioni corrispondenti, 0 altrimenti.
int prodotto_mod2(const std::vector<int>& S, const std::vector<int>& P) {
	int somma = 0;
	for (size_t i = 0; i < S.size(); i++) {
		somma = somma ^ (S[i] & P[i]); // XOR accumulato = somma mod 2 dei prodotti bit a bit
	}
	return somma;
}

// Differenza simmetrica tra vettori booleani
// // (S diff P)_i = XOR(S_i, P_i). Modifica S in place.
void differenza_simmetrica(std::vector<int>& S, const std::vector<int>& P) {
	for (size_t i = 0; i < S.size(); i++) {
		S[i] = S[i] ^ P[i];
	}
}

// Vettore d'incidenza di un ciclo
// Dato un ciclo come sequenza ordinata di nodi (con il primo ripetuto in coda),
// restituisce il vettore booleano di lunghezza m che ha 1 negli archi percorsi.
// L'incremento e' modulo 2 (^= 1): se un arco fosse percorso due volte i due
// passaggi si annullerebbero, com'e' corretto nell'algebra su F2.
template<typename I> requires std::integral<I>
std::vector<int> vettore_incidenza(const unidirected_graph<I>& grafo, const std::vector<I>& ciclo) {
	std::vector<int> C(grafo.all_edges().size(), 0);
	for (size_t k = 0; k + 1 < ciclo.size(); k++) {
		unidirected_edge<I> arco(ciclo[k], ciclo[k+1]); // il costruttore ordina i nodi
		auto idx = grafo.edge_number(arco);
		if (idx.has_value()) {
			C[idx.value()] ^= 1;
		}
	}
	return C;
}

// BFS su grafo non pesato: cammino minimo tra due nodi
// Restituisce il cammino come sequenza di nodi (partenza ... arrivo), oppure
// nullopt se l'arrivo non e' raggiungibile.
//
// Nel grafo di lifting tutti gli archi hanno lo stesso peso, quindi "cammino
// minimo" significa "cammino con meno archi": la BFS lo trova senza bisogno di
// Dijkstra e della coda di priorita'. (Le note propongono Dijkstra "per esempio";
// con pesi unitari la BFS e' equivalente e piu' semplice.)
template<typename I> requires std::integral<I>
std::optional<std::vector<I>> bfs_cammino_minimo(const unidirected_graph<I>& grafo, I partenza, I arrivo) {
	std::map<I, I> predecessore; // per ricostruire il cammino a ritroso
	predecessore[partenza] = partenza;

	fifo<I> coda; // riuso il contenitore FIFO gia' definito in visite_grafi.hpp
	coda.put(partenza);

	while (!coda.empty()) {
		auto u_opt = coda.get();
		if (u_opt == std::nullopt) {
			break;
		}
		I u = *u_opt;
		if (u == arrivo) {
			break;
		}
		for (I vicino : grafo.neighbours(u)) {
			if (!predecessore.contains(vicino)) {
				predecessore[vicino] = u;
				coda.put(vicino);
			}
		}
	}

	if (!predecessore.contains(arrivo)) {
		return std::nullopt; // arrivo non raggiungibile
	}

	// ricostruzione del cammino da arrivo a partenza, poi lo inverto
	std::vector<I> cammino;
	I x = arrivo;
	while (x != partenza) {
		cammino.push_back(x);
		x = predecessore[x];
	}
	cammino.push_back(partenza);
	std::reverse(cammino.begin(), cammino.end());
	return cammino;
}

// Ciclo minimo non ortogonale al testimone S
// Trova il ciclo di peso minimo Ci tale che <Ci, S> = 1, cioe' che attraversa un
// numero DISPARI di archi attivi in S.
//
template<typename I> requires std::integral<I>
std::vector<I> ciclo_minimo_con_parita(const unidirected_graph<I>& grafo, const std::vector<int>& S) {

	std::vector<I> nodi = grafo.all_nodes();

	// M: offset per la copia "meno". Deve superare ogni etichetta di nodo.
	I M = nodi.back() + 1; // all_nodes() e' ordinato, quindi back() e' il massimo

	// costruzione del grafo di lifting
	unidirected_graph<I> lifting;
	for (const auto& arco : grafo.all_edges()) {
		I u = arco.from();
		I w = arco.to();
		auto idx = grafo.edge_number(arco);
		if (idx.has_value() && S[idx.value()] == 1) {
			lifting.add_edge(u, w + M);     // incrocio dei livelli
			lifting.add_edge(u + M, w);
		} else {
			lifting.add_edge(u, w);         // stesso livello
			lifting.add_edge(u + M, w + M);
		}
	}

	std::optional<std::vector<I>> migliore;
	for (I v : nodi) {
		auto cammino = bfs_cammino_minimo(lifting, static_cast<I>(v + M), v); // da v- a v+
		if (cammino == std::nullopt) {
			continue;
		}

		// riporto i nodi in G: i nodi del livello "-" (>= M) tornano sottraendo M.
		// Il risultato e' gia' la sequenza ORDINATA del ciclo, che parte e finisce
		// in v: e' cio' che serve per fissare il verso di percorrenza della maglia.
		std::vector<I> ciclo;
		ciclo.reserve(cammino->size());
		for (I x : *cammino) {
			ciclo.push_back(x >= M ? x - M : x);
		}

		if (migliore == std::nullopt || ciclo.size() < migliore->size()) {
			migliore = ciclo;
		}
	}

	return migliore.value(); // se il grafo e' un circuito valido, un ciclo esiste sempre
}

// Funzione principale: algoritmo di De Pina
std::vector<std::vector<int>> algoritmo_depina(const std::map<std::string, valori_numerici>& circuito) {

	// 1. costruzione del grafo del circuito (identica ad algoritmo_dfs)
	unidirected_graph<int> grafo_circuito;
	for (const auto& [_, nodi] : circuito) {
		grafo_circuito.add_edge(nodi.n1, nodi.n2);
	}

	int m = grafo_circuito.all_edges().size();          // numero di archi
	int num_nodi = grafo_circuito.all_nodes().size();   // numero di nodi
	int k = m - num_nodi + 1;                           // numero di cicli fondamentali

	// 2. albero DFS e coalbero (come in algoritmo_dfs, riusando visite_grafi con la LIFO)
	lifo<int> stack;
	unidirected_graph<int> albero_dfs = visite_grafi(grafo_circuito, grafo_circuito.all_nodes()[0], stack);
	unidirected_graph<int> coalbero = grafo_circuito - albero_dfs;

	// 3. inizializzazione dei k testimoni S_i.
	// Il testimone i-esimo vale 1 solo nella posizione dell'i-esimo arco del coalbero.
	// Gli archi del coalbero sono esattamente k.
	std::vector<std::vector<int>> S(k, std::vector<int>(m, 0));
	int i = 0;
	for (const auto& arco : coalbero.all_edges()) {
		auto idx = grafo_circuito.edge_number(arco); // indice nel grafo COMPLETO, non nel coalbero
		if (idx.has_value()) {
			S[i][idx.value()] = 1;
		}
		i++;
	}

	// 4. ciclo principale dell'algoritmo di De Pina
	std::vector<std::vector<int>> maglie;
	maglie.reserve(k);

	for (int idx_i = 0; idx_i < k; idx_i++) {

		// trovo il ciclo minimo C_i non ortogonale al testimone S_i
		std::vector<int> C_i = ciclo_minimo_con_parita(grafo_circuito, S[idx_i]);
		maglie.push_back(C_i);

		// vettore d'incidenza di C_i, per aggiornare i testimoni successivi
		std::vector<int> inc = vettore_incidenza(grafo_circuito, C_i);

		// ortogonalizzazione: rendo ortogonali a C_i tutti i testimoni successivi
		for (int idx_j = idx_i + 1; idx_j < k; idx_j++) {
			if (prodotto_mod2(inc, S[idx_j]) == 1) {
				differenza_simmetrica(S[idx_j], S[idx_i]);
			}
		}
	}

	return maglie;
}

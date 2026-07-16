#include <cstdlib>
#include "grafo.hpp"
#include "visite_grafi.hpp"
#include "funzioni.hpp"
#include "gradiente_coniugato.hpp"
#include "algoritmo_dfs.hpp"
#include "algoritmo_depina.hpp"


int main(const int argc, const char *argv[]){
	
	info_circuito ic = lettura_netlist(argc, argv);
	
	if (ic.controllo == false){ //termino il programma se ho avuto un errore in fase di lettura/apertura file
		return EXIT_FAILURE;
	}
	
	// Algoritmo di De Pina per cicli minimi
	// std::vector<std::vector<int>> maglie = algoritmo_depina(ic.circuito);
	
	// Algoritmo basato su DFS
	std::vector<std::vector<int>> maglie = algoritmo_dfs(ic.circuito);
	
	int num_maglie = maglie.size();

	dati_sistema dati = calcola_sistema(ic.circuito, ic.ordine_componenti, maglie, ic.resistori, num_maglie);
	Eigen::MatrixXd B = dati.B;
	Eigen::MatrixXd R = dati.R;
	Eigen::VectorXd v = dati.v;
	
	Eigen::MatrixXd A = B.transpose() * R * B;
	
	// Soluzione sistema lineare
	const int it_extra = 0;
	const int it_max = num_maglie + it_extra;
	//Proprieta' del gc: converge alla soluzione esatta in al piu' n iterazioni dove n e' la dimensione della matrice A (numero di maglie)
	//In virgola mobile, a volte potrebbe essere necessaria qualche iterazione in piu' (per questo ho inserito it_extra)
	const double tol = 1.0e-13;
	risultati_gc esito_sistema = gradiente_coniugato(A, v, it_max, tol);
	
	//stampa errore se il metodo non ha raggiunto la convergenza
	if (!esito_sistema.convergenza) {
		std::cerr << "Errore: il metodo del gradiente coniugato non ha raggiunto la convergenza (Iterazioni: " << esito_sistema.iterazioni << ").\n"
                  << "Verificare la correttezza del grafo.\n";
		return EXIT_FAILURE;
	}

	//calcolo delle tensioni
	Eigen::VectorXd correnti_maglie = esito_sistema.soluzione;
	Eigen::VectorXd correnti_resistori = B * correnti_maglie;
	Eigen::VectorXd tensioni_resistori = R * correnti_resistori;
	
	// stampa delle tensioni e delle correnti di ogni resistore
	stampa(ic.ordine_componenti, tensioni_resistori, correnti_resistori);
	
	return EXIT_SUCCESS;
}
	
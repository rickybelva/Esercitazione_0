#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <Eigen/Dense>
#include <concepts>
#include <algorithm>
#include <set>
#include <utility>


// Lettura della netlist

//valori numerici (modulo della resistenza o della tensione, nodo di partenza, nodo di arrivo)
struct valori_numerici {
    double modulo;
    int n1;
    int n2;
};

//struttura che memorizza le informazioni del circuito:
//-mappa contenente il nome del componente e i relativi valori numerici (la stringa R# o V# si può usare come chiave in quanto non ci sono componenti ripetuti)
//-vettore contenente i nomi dei componenti ordinati come nella netlist
//-numero di resistori
//-variabile booleana di controllo
struct info_circuito {
    std::map<std::string, valori_numerici> circuito;
	std::vector<std::string> ordine_componenti;
    int resistori;
    bool controllo;
};

//funzione che legge la netlist e restituisce le informazioni del circuito
info_circuito lettura_netlist(const int argc, const char *argv[]){
	info_circuito ic;
	ic.resistori = 0;
	ic.controllo = true;
	  
	if (argc!=2){ //devo ottenere in input una sola stringa contenente il nome del file e questo si verifica solo se argc=2.
		std::cerr << "Errore, inserire il nome di un file.\n";
		ic.controllo = false;
		return ic;
		}
	
	std::string filename;
	filename = argv[1];
	std::ifstream ifs(filename);
	
	//Gestione dell'errore nella lettura del file
	if (ifs.fail()) {
		std::cerr << "Impossibile aprire il file '" << filename << "'.\n";
		ic.controllo = false;
	return ic;
	}
	
	std::cout << "File aperto correttamente.\n\n";
	
	std::string chiave;
	valori_numerici val;
	std::string linea;
	
	while (std::getline(ifs, linea)) {
		
		//elimino il carattere '\r' (carriage return)
		if (!linea.empty() && linea.back() == '\r') {
			linea.pop_back();
		}
		
		//controllo che la riga non sia vuota e che non inizi con i caratteri ' ', '\t', '\r', '\n', altrimenti passo alla linea successiva
		if (linea.empty() || linea.find_first_not_of(" \t\r\n") == std::string::npos) continue;
		
		std::istringstream iss(linea);
		
		//caso in cui la riga non è vuota ma il formato è sbagliato
		if (!(iss >> chiave >> val.modulo >> val.n1 >> val.n2)) {
			std::cerr << "Attenzione, riga malformata ignorata: '" << linea << "'\n";
			continue;
		}
		
		//caso in cui la chiave (ossia il nome del componente) non comincia per 'R' o per 'V'
		if (chiave[0] != 'R' && chiave[0] != 'V') {
			std::cerr << "Attenzione, componente sconosciuto '" << chiave << "' ignorato nella riga: '" << linea << "'\n";
			continue;
		}
		
		ic.circuito[chiave] = {val.modulo, val.n1, val.n2}; //associo a ogni componente i relativi valori numerici
		ic.ordine_componenti.push_back(chiave); //memorizzo l'ordine dei componenti
		if (chiave[0] == 'R') {
			ic.resistori++; //memorizzo il numero di resistori
		}
	}
	
	return ic;
};


// Calcolo Sistema

struct dati_sistema {
	Eigen::MatrixXd B;
	Eigen::MatrixXd R;
	Eigen::VectorXd v;
};


template<typename I> requires std::integral<I>
dati_sistema calcola_sistema(const std::map<std::string, valori_numerici>& circuito, const std::vector<std::string>& ordine_componenti,
	const std::vector<std::vector<I>>& maglie, int num_resistori, int num_maglie) {
	
	Eigen::MatrixXd B = Eigen::MatrixXd::Zero(num_resistori, num_maglie);
	Eigen::MatrixXd R = Eigen::MatrixXd::Zero(num_resistori, num_resistori);
	Eigen::VectorXd v = Eigen::VectorXd::Zero(num_maglie); 
	
	// Calcolo tutti gli archi delle maglie per non doverle ricalcolare ad ogni iterazione
    std::vector<std::set<std::pair<I, I>>> archi_maglie(num_maglie); // vettore di dimensione num_maglie contenente insiemi di archi
    for (int j = 0; j < num_maglie; j++) {
        for (size_t k = 0; k + 1 < maglie[j].size(); k++) {
            archi_maglie[j].insert({maglie[j][k], maglie[j][k+1]});
        }
    }
	
	int i_res = 0; // indice resistore (riga)
	
	//scorro sui nomi dei componenti
	for (const std::string& key : ordine_componenti) {
        
		//valori di ogni componente
		auto valori = circuito.at(key); 
        I n1 = valori.n1;
        I n2 = valori.n2;
		
		//se ho un resistore
		if (key[0] == 'R') {
            // Aggiorno la diagonale di R inserendo il modulo della resistenza 
            R(i_res, i_res) = valori.modulo;
			
			// Il verso di percorrenza dell’arco e' sempre dal nodo di indice minore a quello di indice maggiore
			I r_start = std::min(n1, n2);
			I r_end = std::max(n1, n2);
			
			for (int j = 0; j < num_maglie; j++) { // j indice maglia (colonna)
				
				// Verso concorde al grafo --> inserisco +1
                if (archi_maglie[j].contains({r_start, r_end})) {
                    B(i_res, j) = 1; 
                }
				
				// Verso discorde al grafo --> inserisco -1
                else if (archi_maglie[j].contains({r_end, r_start})) {
                    B(i_res, j) = -1;
                }
            }
			
            i_res++;
		}	
		
		// se ho un generatore di tensione
		else if (key[0] == 'V') {
			for (int j = 0; j < num_maglie; j++) {
			
				// Da - a + : contributo positivo
				if (archi_maglie[j].contains({n2, n1})) { 
					v(j) += valori.modulo;
				}
               
				// Da + a - : contributo negativo
				else if (archi_maglie[j].contains({n1, n2})) { 
					v(j) -= valori.modulo;
				}
			}	
		}
	}
	
	dati_sistema risultato;
	risultato.B = B;
	risultato.R = R;
	risultato.v = v;
	
	return risultato;
}


// Stampa delle tensioni e delle correnti di ciascun resistore

void stampa(const std::vector<std::string>& ordine_componenti, const Eigen::VectorXd& tensioni, const Eigen::VectorXd& correnti) {
	int i_res=0;
	std::cout.precision(4); //imposto le cifre significative
	for (const std::string& key : ordine_componenti) {
        if (key[0] == 'R') {
			std::cout << key << ": V = " << tensioni(i_res) << " volts, I = " << correnti(i_res) << " amps.\n";
			i_res++;
		}
	}
}
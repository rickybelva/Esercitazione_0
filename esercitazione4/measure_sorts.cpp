#include <iostream>
#include <vector>
#include <algorithm> // per std::sort
#include <iomanip>   // per std::setw--> funzione che sereve ad allineare l'output, ho trovato informazioni sul sito stack overflow 
#include <cstdlib>   // per EXIT_SUCCESS
#include "randfiller.h"
#include "timecounter.h"
#include "Sorts.hpp" 

int main() {
    randfiller rf;
    timecounter tc;

    std::cout << "--- BENCHMARK ALGORITMI DI ORDINAMENTO ---\n";
    std::cout << "Confronto dei tempi (in secondi) su vettori di interi random.\n\n";

    //Intestazione tabella: usiamo std::setw per allineare le colonne
    std::cout << std::setw(8)  << "Size" 
              << std::setw(18) << "Bubble Sort" 
              << std::setw(18) << "Insertion Sort" 
              << std::setw(18) << "Selection Sort" 
              << std::setw(18) << "std::sort" << "\n";
    std::cout << std::string(80, '-') << "\n";

    //Ciclo per le lunghezze richieste: 4, 8, 16, ..., 8192
    for (int size = 4; size <= 8192; size *= 2) {
        
        //Creazione del vettore originale (disordinato)
        std::vector<int> original_vec;
        original_vec.resize(size);
        rf.fill(original_vec, -10000, 10000);

        //Creazione delle copie per ogni algoritmo
        std::vector<int> v_bubble = original_vec;
        std::vector<int> v_insertion = original_vec;
        std::vector<int> v_selection = original_vec;
        std::vector<int> v_std_sort = original_vec;

        //Misurazione Bubble Sort
        tc.tic();
        bubble_sort(v_bubble);
        double t_bubble = tc.toc();

        //Misurazione Insertion Sort
        tc.tic();
        insertion_sort(v_insertion);
        double t_insertion = tc.toc();

        //Misurazione Selection Sort
        tc.tic();
        selection_sort(v_selection);
        double t_selection = tc.toc();

        //Misurazione std::sort
        tc.tic();
        std::sort(v_std_sort.begin(), v_std_sort.end());
        double t_std = tc.toc();

        // Stampa dei risultati con formattazione più elegante 
        std::cout << std::setw(8)  << size 
                  << std::setw(18) << t_bubble 
                  << std::setw(18) << t_insertion 
                  << std::setw(18) << t_selection 
                  << std::setw(18) << t_std << "\n";
    }

    return EXIT_SUCCESS;
}
#pragma once
#include <iostream>
#include <optional>
#include <vector> //per usare la classe dei vettori dinamici
//Implementazione algoritmo bubblesort
template<typename T>
void bubble_sort(std::vector<T>& vec){
	int N=vec.size();
	for(int i=0; i<=N-2;i++){
		for(int j=N-1;j>=i+1;--j){
			if(vec[j]<vec[j-1]){
				T scambio=vec[j];
				vec[j]=vec[j-1];
				vec[j-1]=scambio;
			}
		}
		
	}
		
}

//Implementazione algoritmo selectionsort
template<typename T>
void selection_sort(std::vector<T>& vec){
	int N=vec.size();
	for(int i=0; i<=N-2;++i){
		int min_index=i;
		for(int j=i+1;j<=N-1;++j){
			if (vec[j]<vec[min_index]){
				min_index=j;
			}
		}
		if (min_index!=i){
			T scambio=vec[min_index];
			vec[min_index]=vec[i];
			vec[i]=scambio;
		}
	}
}

//Implementazione algoritmo insertionsort
template<typename T>
void insertion_sort(std::vector<T>& vec){
	int N=vec.size();
	for(int i=1; i<=N-1; ++i){
		T value=vec[i];
		int j=i-1;
		while(j>=0 && vec[j]>value){
			vec[j+1]=vec[j];
			j=j-1;
		}
		vec[j+1]=value;
	}
}


//implementazione funzione che controlla se il vettore è effettivamente ordinato
template<typename T>
// 'bool' perché la risposta è un semplice Vero o Falso
// 'const std::vector<T>&' perché si legge solo il vettore senza modificarlo
bool is_sorted(const std::vector<T>& vec) {
    int N = vec.size();
    for(int i = 0; i < N - 1; ++i) {
        if(vec[i] > vec[i + 1]) {
            return false;
        }
    }
    return true; 
}
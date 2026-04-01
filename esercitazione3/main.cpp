#include "rational.hpp"
#include <iostream>
#include <concepts>
#include <numeric>
#include <cmath>

int main(void) {
	std::cout << "Inserire il numeratore del primo numero: ";
	int n1;
	std::cin >> n1;
	std::cout << "Inserire il denominatore del primo numero: ";
	int d1;
	std::cin >> d1;
	rational<int> r1(n1,d1);
	std::cout << "Inserire il numeratore del secondo numero: ";
	int n2;
	std::cin >> n2;
	std::cout << "Inserire il denominatore del secondo numero: ";
	int d2;
	std::cin >> d2;
	rational<int> r2(n2,d2);
	
	std::cout << "Il primo numero inserito è " << r1 << "\n";
	std::cout << "Il secondo numero inserito è " << r2 << "\n";

	rational<int> somma = r1 + r2;
	rational<int> differenza = r1 - r2;
	rational<int> prodotto = r1 * r2;
	rational<int> divisione = r1 / r2;
	std::cout << "La somma è " << somma << "\n";
	std::cout << "La differenza è " << differenza << "\n";
	std::cout << "Il prodotto è " << prodotto << "\n";
	std::cout << "Il quoziente è " << divisione << "\n";


	return 0;
}
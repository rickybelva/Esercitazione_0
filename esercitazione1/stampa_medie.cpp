#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Nome del file non trovato";
		return 1;
	}
	std::ifstream ifs(argv[1]);
	if( ifs.fail() ) {
		std::cerr << "Errore nell'apertura del file";
		return 2;
	}
	if( ifs.is_open() ) {
		while( !ifs.eof() ) {
			std::string city;
			double t1;
			double t2;
			double t3;
			double t4;
			ifs >> city >> t1 >> t2 >> t3 >> t4;
			double mean = 0.25*(t1 +  t2 + t3 + t4);
			std::cout << city << " " << mean << "\n";
		}
		return 0;
	}
}
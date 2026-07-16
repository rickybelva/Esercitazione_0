#pragma once
#include <iostream>
#include <Eigen/Dense>

// struttura che restituisce i risultati del metodo del gradiente coniugato
struct risultati_gc {
	Eigen::VectorXd soluzione; 
    unsigned int iterazioni;     
    double norma_residuo;         
    bool convergenza;
};

risultati_gc gradiente_coniugato(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, Eigen::VectorXd& x,
const unsigned int it_max = 1000, const double tol = 1.0e-13) {
		
	//Il metodo del gradiente coniugato si può utilizzare in quanto la matrice A = B(^t) * R * B e' sicuramente simmetrica definita positiva
	//Dunque ometto il codice che controlla che A sia simmetrica
	
	// Controllo che A sia quadrata e che abbia le dimensioni compatibili con i vettori b e x
	if (A.rows() != A.cols() || A.rows() != b.size() || x.size() != b.size()) {
		std::cerr << "Errore: dimensione dati incompatibile \n";
		return {x, 0, 0.0, false};
	}
		
	risultati_gc r;
  
	Eigen::VectorXd res = b - A * x;
	Eigen::VectorXd p = res;
	
	// Se x e gia' soluzione esci
	if (res.norm() < tol) {
		return {x,  0, 0.0, true};
	}
	
	unsigned int it = 0;

	while (it < it_max && res.norm() > tol) {
		
		Eigen::VectorXd Ap = A * p;
		auto den = p.dot(Ap);
		
		//controllo denominatore
		if (std::abs(den) < 1e-15) {
            std::cerr << "Errore: denominatore nullo all'iterazione " << it << ".\n";
            break;
        }
		
		double alpha = p.dot(res) / den;		
		x = x + alpha * p;
		
		res = b - A * x;
		double beta = (p.dot(A*res))/den;
		p = res - beta * p;
		
		it++;
		
	}
	
	r.soluzione = x;
	r.iterazioni = it;
	r.norma_residuo = res.norm();
	r.convergenza = (res.norm() <= tol);
	
	return r;
}

// Se non si specifica il vettore di partenza si parte dal vettore nullo
risultati_gc gradiente_coniugato(const Eigen::MatrixXd& A, const Eigen::VectorXd& b,
const unsigned int it_max = 1000, const double tol = 1.0e-13) {
	
	Eigen::VectorXd x0 = Eigen::VectorXd::Zero(b.size());
	return gradiente_coniugato(A, b, x0, it_max, tol);

}
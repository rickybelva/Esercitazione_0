#pragma once
#include <iostream>
#include <Eigen/Dense>

//Utilizzo una struct per poter restituire tutti risultati dell'algoritmo
struct RisultatoCG {
    Eigen::VectorXd x;          //soluzione trovata
    int iterazioni;             //numero di iterazioni eseguite
    double norma_residuo;       //norma del residuo finale
    bool convergenza;           //true se ha raggiunto la tolleranza
};

//Implementazione del metodo del Gradiente Coniugato

RisultatoCG gradiente_coniugato(const Eigen::VectorXd& x_0,
                                const Eigen::MatrixXd& A,
                                const Eigen::VectorXd& b,
                                const int k_max,
                                const double tol)
{
    //Controllo sulle dimensioni
    if (x_0.size() != b.size() || x_0.size() != A.rows() || A.rows() != A.cols()) {
        std::cerr << "Errore: dimensioni dei dati in input incompatibili.\n";
        return {Eigen::VectorXd(), 0, 0.0, false};
    }
    //Controllo sul fatto che A sia simmetrica
    if (!A.isApprox(A.transpose())) {
        std::cerr << "ATTENZIONE: la matrice passata non e' simmetrica!\n";
    }


    Eigen::VectorXd x = x_0;
    Eigen::VectorXd r = b - A * x;
    Eigen::VectorXd p = r;
    int k = 0;

    while (r.norm() > tol && k < k_max) {

        Eigen::VectorXd Ap = A * p;         
        double den = p.dot(Ap);             

        //Controllo denominatore 
        if (std::abs(den) < 1e-15) {
            std::cerr << "Errore: denominatore nullo all'iterazione " << k << ".\n";
            break;
        }

        double alpha = p.dot(r) / den;     
        x = x + alpha * p;                  
        r = b - A * x;                      

        double beta = Ap.dot(r) / den;

        p = r - beta * p;                   // p_{k+1} = r_{k+1} - beta_k * p_k

        k++;
    }

    //Risultato
    bool convergenza = (r.norm() <= tol);

    return {x, k, r.norm(), convergenza};
}
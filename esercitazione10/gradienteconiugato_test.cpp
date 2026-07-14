#include <iostream>
#include <Eigen/Dense>
#include "Gradienteconiugato.hpp"

//Costruisce una matrice simmetrica positiva definita di dimensione n come B^T * B
Eigen::MatrixXd costruisci_matrice_SPD(int n)
{
    Eigen::MatrixXd B = Eigen::MatrixXd::Random(n, n);
    return B.transpose() * B + Eigen::MatrixXd::Identity(n, n);
}

int main()
{
    const int    k_max = 10000;
    const double tol   = 1.0e-10;
    
    //Test 1: sistema 3x3, soluzione esatta x = [1, 1, 1]^T
    
    std::cout << "Test 1: sistema 3x3" << std::endl;
    {
        int n = 3;
        Eigen::MatrixXd A    = costruisci_matrice_SPD(n);
        Eigen::VectorXd x_ex = Eigen::VectorXd::Ones(n);
        Eigen::VectorXd b    = A * x_ex;
        Eigen::VectorXd x_0  = Eigen::VectorXd::Zero(n);

        RisultatoCG res = gradiente_coniugato(x_0, A, b, k_max, tol);

        double err_rel = (x_ex - res.x).norm() / x_ex.norm();

        std::cout << std::scientific;
        std::cout << "Iterazioni:      " << res.iterazioni    << " / " << k_max  << std::endl;
        std::cout << "Norma residuo:   " << res.norma_residuo << " (tol = " << tol << ")" << std::endl;
        std::cout << "Errore relativo: " << err_rel           << std::endl;
        std::cout << "Convergenza:     " << (res.convergenza ? "SI" : "NO") << std::endl;
        std::cout << std::endl;
    }
    
    //Test 2: sistema 50x50, soluzione esatta x = [1, ..., 1]^T
    
    std::cout << "Test 2: sistema 50x50 " << std::endl;
    {
        int n = 50;
        Eigen::MatrixXd A    = costruisci_matrice_SPD(n);
        Eigen::VectorXd x_ex = Eigen::VectorXd::Ones(n);
        Eigen::VectorXd b    = A * x_ex;
        Eigen::VectorXd x_0  = Eigen::VectorXd::Zero(n);

        RisultatoCG res = gradiente_coniugato(x_0, A, b, k_max, tol);

        double err_rel = (x_ex - res.x).norm() / x_ex.norm();

        std::cout << std::scientific;
        std::cout << "Iterazioni:      " << res.iterazioni    << " / " << k_max  << std::endl;
        std::cout << "Norma residuo:   " << res.norma_residuo << " (tol = " << tol << ")" << std::endl;
        std::cout << "Errore relativo: " << err_rel           << std::endl;
        std::cout << "Convergenza:     " << (res.convergenza ? "SI" : "NO") << std::endl;
        std::cout << std::endl;
    }

    // Test 3: verifica con soluzione non banale x = [1, 2, 3]^T
  
    std::cout << "Test 3: soluzione non banale" << std::endl;
    {
        int n = 3;
        Eigen::MatrixXd A    = costruisci_matrice_SPD(n);
        Eigen::VectorXd x_ex(n);
        x_ex << 1.0, 2.0, 3.0;
        Eigen::VectorXd b   = A * x_ex;
        Eigen::VectorXd x_0 = Eigen::VectorXd::Zero(n);

        RisultatoCG res = gradiente_coniugato(x_0, A, b, k_max, tol);

        double err_rel = (x_ex - res.x).norm() / x_ex.norm();

        std::cout << std::scientific;
        std::cout << "Iterazioni:      " << res.iterazioni    << " / " << k_max  << std::endl;
        std::cout << "Norma residuo:   " << res.norma_residuo << " (tol = " << tol << ")" << std::endl;
        std::cout << "Errore relativo: " << err_rel           << std::endl;
        std::cout << "Convergenza:     " << (res.convergenza ? "SI" : "NO") << std::endl;
        std::cout << std::endl;
    }

    //Test 4: input con dimensioni incompatibili (test errore)
    
    std::cout << "Test 4: dimensioni incompatibili" << std::endl;
    {
        Eigen::MatrixXd A = Eigen::MatrixXd::Identity(3, 3);
        Eigen::VectorXd b = Eigen::VectorXd::Ones(4);   // dimensione sbagliata
        Eigen::VectorXd x_0 = Eigen::VectorXd::Zero(3);

        RisultatoCG res = gradiente_coniugato(x_0, A, b, k_max, tol);
        std::cout << "Convergenza: " << (res.convergenza ? "SI" : "NO")
                  << "  (atteso: NO)" << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
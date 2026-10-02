#include "ImplicitSolvers1D/CoefficientMatrix1D/CoefficientMatrix1D.hpp"
#include <iomanip>
#include <iostream>
CoefficientMatrix1D::CoefficientMatrix1D
   (
    SolverScheme Scheme_,
    const Mesh1D& Mesh1D_Obj,
    double DiffNumber_)
    :Scheme(Scheme_),
     N(Mesh1D_Obj.Get_N()),
     AValues(N * N,0.0),
     r(DiffNumber_)
    {
        if (Scheme == SolverScheme::Laasonen)
        {   
            //Iterate through the interior nodes
            Create_A_Laasonen();
        }
        else if (Scheme == SolverScheme::CrankNicolson)
        {
            //Iterate through the interior nodes
            Create_A_CrankNicolson();
        }
       
    }

void CoefficientMatrix1D::Create_A_Laasonen() noexcept
{
        // ================
        // Lassonen Method
        // =================================================================================
        // EQ : -r * U[i - 1][n + 1] + (1.0 + 2.0 * r) * U[i][n + 1] - r * U[i + 1][n + 1] = RHS(U_n)
        // RHS(U_n) = U[i][n]
        // Create A for Laasonen Method
        // =================================================================================
        // Iterate through the interior nodes

        for (std::size_t i = 1; i < N - 1; ++i)
        {
            AValues[i*N +(i-1)] = -r; //A[i][i - 1] = -r;
            AValues[i*N + (i)] = 1.0 + 2.0 * r; //A[i][i] = 1.0 + 2.0 * r;
            AValues[i*N + (i+1)] = -r; //A[i][i + 1] = -r;
        }

}

void CoefficientMatrix1D::Create_A_CrankNicolson() noexcept
{
        // =====================================================================================
        // CrankNicolson Method
        // =====================================================================================
        //  EQ : -(r / 2) * U[i - 1]^(n + 1) + (1 + r) * U[i]^(n + 1) - (r / 2) * U[i + 1]^(n + 1)
        //  = (r / 2) * U[i - 1]^n + (1 - r) * U[i]^n + (r / 2) * U[i + 1]^n
        //  Create A for CrankNicolson Method  
        // =====================================================================================

        double r_half = r / 2.0;
        // Iterate through the interior nodes
        for (std::size_t i = 1; i < N - 1; ++i)
        {
        AValues[i*N +(i-1)] = -r_half; //A[i][i - 1] = -r / 2.0;
        AValues[i*N + (i)] = 1.0 + r; //A[i][i] = 1.0 + r;
        AValues[i*N + (i+1)] = -r_half; //A[i][i + 1] = -r / 2.0;
        }

}



void CoefficientMatrix1D::Print() const noexcept
{
    std::cout << std::fixed << std::setprecision(4);

    for (std::size_t j = 0; j < N; ++j)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            std::cout << std::setw(15) << AValues[j*N + i];
        }

        std::cout << '\n';
    }
}


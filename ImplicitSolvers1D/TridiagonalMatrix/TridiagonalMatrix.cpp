#include "ImplicitSolvers1D/TridiagonalMatrix/TridiagonalMatrix.hpp"

TridiagonalMatrix::TridiagonalMatrix(        
        SolverScheme Scheme_,
        const Mesh1D& Mesh1D_Obj,
        double DiffNumber_)
    :Scheme(Scheme_),
     N(Mesh1D_Obj.Get_N()),
     LowerDiagonal(N, 0.0),
     MainDiagonal(N, 0.0),
     UpperDiagonal(N, 0.0),
     r(DiffNumber_)
{
    if (Scheme == SolverScheme::Laasonen)
    {
        Create_A3_Laasonen();
    }
    else if (Scheme == SolverScheme::CrankNicolson)
    {
        Create_A3_CrankNicolson();
    }
}

 void TridiagonalMatrix::Create_A3_Laasonen() noexcept
 {
     // ================
     // Lassonen Scheme
     // ================
     // EQ : -r * U[i - 1][n + 1] + (1.0 + 2.0 * r) * U[i][n + 1] - r * U[i + 1][n + 1] = RHS(U_n)
     // RHS(U_n) = U[i][n]
     // Create : 
     // *LowerDiagonal For CrankNicolson Scheme
     // *MainDiagonal For CrankNicolson Scheme
     // *UpperDiagonal For CrankNicolson Scheme
     // ========================================

     // Iterate through the interior nodes
     for (std::size_t i = 1; i < N - 1; ++i)
     {
         LowerDiagonal[i] = -r; //A[i][i - 1] = -r;

         MainDiagonal[i] = 1.0 + 2.0 * r; //A[i][i] = 1.0 + 2.0 * r;

         UpperDiagonal[i] = -r; //A[i][i + 1] = -r;
     }
 }

 void TridiagonalMatrix::Create_A3_CrankNicolson() noexcept
 {
     // =====================================================================================
     // CrankNicolson Scheme
     // =====================================================================================
     //  EQ : -(r / 2) * U[i - 1]^(n + 1) + (1 + r) * U[i]^(n + 1) - (r / 2) * U[i + 1]^(n + 1)
     //  = (r / 2) * U[i - 1]^n + (1 - r) * U[i]^n + (r / 2) * U[i + 1]^n
     //  Create : 
     //     *LowerDiagonal For CrankNicolson Scheme
     //     *MainDiagonal For CrankNicolson Scheme
     //     *UpperDiagonal For CrankNicolson Scheme
     // =================================================
         double r_half = r / 2.0;
         
         // Iterate through the interior nodes
         for (std::size_t i = 1 ; i < N-1; ++i)
         {
             //LowerDiagonal For CrankNicolson Scheme
             LowerDiagonal[i] = -r_half; // A[i][i - 1] = -r / 2.0;
             //MainDiagonal For CrankNicolson Scheme
             MainDiagonal[i] = 1.0 + r; //A[i][i] = 1.0 + r;
             //UpperDiagonal For CrankNicolson Scheme
             UpperDiagonal[i] = -r_half; //A[i][i + 1] = -r / 2.0;
         }
 }  



// IMPORTANT:
// LowerDiagonalValues[0] and UpperDiagonalValues[numNodes - 1] are unused
// placeholder elements. They only exist so that L[i], d[i], and u[i]
// can all be accessed using the same row index i.
//
// However, UpperDiagonalValues[0] and LowerDiagonalValues[numNodes - 1]
// are NOT unused:
//   - UpperDiagonalValues[0] is the coefficient of U[1] in the first row.
//   - LowerDiagonalValues[numNodes - 1] is the coefficient of U[numNodes - 2]
//     in the last row.
//
// For Dirichlet boundary conditions, the first and last matrix rows must
// directly enforce:
//   U[0] = leftBoundaryValue
//   U[numNodes - 1] = rightBoundaryValue
//
// Therefore, the boundary rows must be identity rows:
//   First row: d[0] = 1.0 and u[0] = 0.0
//   Last row:  L[numNodes - 1] = 0.0 and d[numNodes - 1] = 1.0
//
// The Crank-Nicolson coefficients L = -r/2, d = 1+r, and u = -r/2
// are valid only for the internal rows i = 1, ..., numNodes - 2.
// Do not assign -r/2 to u[0] or L[numNodes - 1], because doing so
// couples the prescribed boundary values to their neighboring nodes
// and prevents the matrix from directly enforcing the Dirichlet BCs.



#include "Discretization/Implicit/CrankNicolson/CrankNicolsonAssembler.hpp"
#include <cstddef>
namespace Discretization::Implicit::CrankNicolson
{
    void AssembleMatrix(CoefficientMatrix& A, double diffNumber) noexcept
    {
        // =====================================================================================
        // Crank-Nicolson Scheme
        // EQ : -(r / 2) * U[i - 1]^(n + 1) + (1 + r) * U[i]^(n + 1) - (r / 2) * U[i + 1]^(n + 1)
        // = (r / 2) * U[i - 1]^n + (1 - r) * U[i]^n + (r / 2) * U[i + 1]^n
        // =====================================================================================
        const std::size_t N = A.GetRows();
        const double r_half = diffNumber / 2.0;

        // Iterate through the interior nodes
        for (std::size_t i = 1; i < N - 1; ++i)
        {
            A[i][i - 1] = -r_half;
            A[i][i]     = 1.0 + diffNumber;
            A[i][i + 1] = -r_half;
        }
    }

    void AssembleMatrix(TridiagonalMatrix& A, double diffNumber) noexcept
    {
        // =====================================================================================
        // Crank-Nicolson Scheme for Tridiagonal Matrix
        // =====================================================================================
        const std::size_t N = A.GetMSize();
        const double r_half = diffNumber / 2.0;

        // Iterate through the interior nodes
        for (std::size_t i = 1; i < N - 1; ++i)
        {
            A.L(i) = -r_half;           // Sub-diagonal
            A.M(i) = 1.0 + diffNumber;  // Main diagonal
            A.U(i) = -r_half;           // Super-diagonal
        }
    }

    void AssembleRHS(RHS& rhs, const Field& field_n, double diffNumber) noexcept
    {
        // =====================================================================================
        // Crank-Nicolson Scheme RHS Assembly
        // =====================================================================================
        const double r_half = diffNumber / 2.0;
        const std::size_t N = field_n.GetSize();

        // Iterate through the interior nodes
        for (std::size_t i = 1; i < N - 1; ++i)
        {
            const double rhs_value = (r_half) * field_n[i - 1]
                                   + (1.0 - diffNumber) * field_n[i]
                                   + (r_half) * field_n[i + 1];

            rhs[i] = rhs_value;
        }
    }
}

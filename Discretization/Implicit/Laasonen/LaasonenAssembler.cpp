#include "Discretization/Implicit/Laasonen/LaasonenAssembler.hpp"
#include <cstddef>
namespace Discretization::Implicit::Laasonen
{
    void AssembleMatrix(CoefficientMatrix& A, double diffNumber) noexcept
    {
        // =================================================================================
        // Laasonen Scheme (Fully Implicit / Backward Time Central Space)
        // EQ : -r * U[i - 1][n + 1] + (1.0 + 2.0 * r) * U[i][n + 1] - r * U[i + 1][n + 1] = RHS(U_n)
        // =================================================================================
        const std::size_t N = A.GetRows();
        const double r = diffNumber;

        // Iterate through the interior nodes
        for (std::size_t i = 1; i < N - 1; ++i)
        {
            A[i][i - 1] = -r;
            A[i][i]     = 1.0 + 2.0 * r;
            A[i][i + 1] = -r;
        }
    }

    void AssembleMatrix(TridiagonalMatrix& A, double diffNumber) noexcept
    {
        // =================================================================================
        // Laasonen Scheme for Tridiagonal Matrix
        // =================================================================================
        const std::size_t N = A.GetMSize();
        const double r = diffNumber;

        // Iterate through the interior nodes
        for (std::size_t i = 1; i < N - 1; ++i)
        {
            A.L(i) = -r;             // Sub-diagonal
            A.M(i) = 1.0 + 2.0 * r;  // Main diagonal
            A.U(i) = -r;             // Super-diagonal
        }
    }

    void AssembleRHS(RHS& rhs, const Field& field_n) noexcept
    {
        // =================================================================================
        // Laasonen Scheme RHS: RHS = U[i][n]
        // =================================================================================
        const std::size_t N = field_n.GetSize();

        // Iterate through the interior nodes
        for (std::size_t i = 1; i < N - 1; ++i)
        {
            rhs[i] = field_n[i];
        }
    }
}

#include "Discretization/Implicit/Boundary/EnforceBoundary.hpp"
#include <cstddef>
#include <stdexcept>
namespace Discretization::Implicit::Boundary
{
    namespace
    {
        // Helper to validate boundary type and avoid code repetition
        inline void ValidateDirichlet(const SimulationBoundaries& Boundaries_Obj)
        {
            if (Boundaries_Obj.GetType() == BoundaryType::Dirichlet)
            {
                return;
            }
            if (Boundaries_Obj.GetType() == BoundaryType::Neumann ||
                Boundaries_Obj.GetType() == BoundaryType::Robin)
            {
                throw std::invalid_argument(
                    "Boundary Conditions: Neumann and Robin Boundary Conditions are not Implemented yet.");
            }
        }
    } // namespace

    void Enforce(
        ::CoefficientMatrix& A_Obj,
        const SimulationBoundaries& Boundaries_Obj)
    {
        ValidateDirichlet(Boundaries_Obj);

        const std::size_t N = A_Obj.GetRows();
        if (N < 2)
        {
            return;
        }

        // Apply Dirichlet boundary condition (1.0 * u_boundary = RHS)
        A_Obj[0][0]         = 1.0;
        A_Obj[N - 1][N - 1] = 1.0;
    }

    void Enforce(
        ::TridiagonalMatrix& A3_Obj,
        const SimulationBoundaries& Boundaries_Obj)
    {
        ValidateDirichlet(Boundaries_Obj);

        const std::size_t N = A3_Obj.GetMSize();
        if (N < 2)
        {
            return;
        }

        // Apply Dirichlet boundary condition To MainDiagonal Values
        A3_Obj.M(0)     = 1.0;
        A3_Obj.M(N - 1) = 1.0;

        // Apply Dirichlet boundary condition To UpperDiagonal Values
        A3_Obj.U(0)     = 0.0;
        A3_Obj.U(N - 1) = 0.0;

        // Apply Dirichlet boundary condition To LowerDiagonal Values
        A3_Obj.L(0)     = 0.0;
        A3_Obj.L(N - 1) = 0.0;
    }

    void Enforce(
        ::RHS& RHS_Obj,
        const Field& Field_Obj,
        const SimulationBoundaries& Boundaries_Obj)
    {
        ValidateDirichlet(Boundaries_Obj);

        const std::size_t N = RHS_Obj.GetSize();
        if (Field_Obj.GetSize() != N)
        {
            throw std::invalid_argument(
                "Field and RHS sizes must match when enforcing boundary conditions.");
        }

        if (N < 2)
        {
            return;
        }

        // Apply Dirichlet boundary values to extremities
        RHS_Obj[0]     = Field_Obj[0];
        RHS_Obj[N - 1] = Field_Obj[N - 1];
    }
} // namespace Discretization::Implicit::Boundary

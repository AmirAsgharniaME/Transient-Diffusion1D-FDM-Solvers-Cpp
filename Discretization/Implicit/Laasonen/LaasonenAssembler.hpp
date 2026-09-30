#pragma once

#include "LinearSolvers/Matrix/CoefficientMatrix.hpp"
#include "LinearSolvers/Matrix/TridiagonalMatrix.hpp"
#include "LinearSolvers/Matrix/RHS.hpp"
#include "Core/Field/Field.hpp"

namespace Discretization::Implicit::Laasonen
{
    // Assemble dense coefficient matrix for Gaussian Elimination
    void AssembleMatrix(CoefficientMatrix& A, double diffNumber) noexcept;

    // Assemble compact tridiagonal matrix for Thomas Algorithm (TDMA)
    void AssembleMatrix(TridiagonalMatrix& A, double diffNumber) noexcept;

    // Assemble Right-Hand Side (RHS) vector
    void AssembleRHS(RHS& rhs, const Field& field_n) noexcept;
}

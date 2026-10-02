#pragma once
#include "ImplicitSolvers1D/TridiagonalMatrix/TridiagonalMatrix.hpp"
#include "ImplicitSolvers1D/RHS1D/RHS1D.hpp"
#include "Core1D/Field1D/Field1D.hpp"

class ThomasAlgorithm
{
public:
    // (Stateless Class)
    ThomasAlgorithm() = delete;


    static void Solve_nPlus1(
        TridiagonalMatrix& A3_Obj,
        RHS1D& RHS1D_Obj,
        Field1D& Solution);

private:
    // (Forward Sweep)
    static void ForwardElimination(
        TridiagonalMatrix& TridiagonalMatrix,
        RHS1D& RHS1D_Obj);

    // (Back Substitution)
    static void BackSubstitution(
        const TridiagonalMatrix& TridiagonalMatrix,
        const RHS1D& RHS1D_Obj,
        Field1D& Solution);
};

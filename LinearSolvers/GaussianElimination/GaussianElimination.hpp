#pragma once

#include <cstddef>
#include "ImplicitSolvers1D/CoefficientMatrix1D/CoefficientMatrix1D.hpp"
#include "ImplicitSolvers1D/RHS1D/RHS1D.hpp"
#include "Core1D/Field1D/Field1D.hpp"


//Gaussian Elimination with Partial Row Pivoting followed by Back Substitution
class GaussianElimination
{

public:

 GaussianElimination() = delete;   //Creating an object of this class is prohibited.
    //A_Obj Solution = RHS1D
    static void Solve_nPlus1(
        CoefficientMatrix1D& A_Obj,
        RHS1D& RHS1D_Obj,
        Field1D& Solution);



private:
    static void ForwardElimination(
        CoefficientMatrix1D& A_Obj,
        RHS1D& RHS1D_Obj);
/*
 * Back Substitution Formula:
 * 
 *   x[i] = ( b[i] - Sum_{j = i+1}^{n-1} ( a[i][j] * x[j] ) ) / a[i][i]
 * 
 * Execution Steps:
 *   1. Initialize 'Sum' as b[i] (RHS1D value).
 *   2. Subtract the product of known variables: Sum -= a[i][j] * x[j] (for j from i+1 to n-1).
 *   3. Calculate the final value: x[i] = Sum / a[i][i].
 */

    static void BackSubstitution(
        const CoefficientMatrix1D& A_Obj,
        const RHS1D& RHS1D_Obj,
        Field1D& Solution);

    [[nodiscard]] static std::size_t FindPivotRowIndex(
        const CoefficientMatrix1D& A_Obj,
        const std::size_t PivotIndex);

    static void SwapRows(
        CoefficientMatrix1D& A_Obj,
        RHS1D& RHS1D_Obj,
        std::size_t FirstRowIndex,
        std::size_t SecondRowIndex);
};

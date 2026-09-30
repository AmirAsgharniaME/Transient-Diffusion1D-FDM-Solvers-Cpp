#pragma once

#include <array>
#include "Core1D/Field1D/Field1D.hpp"

namespace  FTCS
{
   void Solve_nPlus1(
    const Field1D& Field1D_n_Obj,
    Field1D& Field1D_nPlus1_Obj, 
    double DiffNumber
)noexcept;
}

namespace  DUFORT_FRANKEL
{
   
   void Solve_nPlus1(
    const Field1D& Field1D_n_Obj,
    const Field1D& Field1D_nminus1_Obj,
    Field1D& Field1D_nplus1_Obj,
    std::array<double, 2>  CoeffsVector_
) noexcept;





//  * Design Choice: Returns std::array<double, 2> instead of std::vector.
//  * - Stack Allocation: Avoids dynamic heap memory management overhead.
//  * - Performance: Guarantees zero runtime allocations, which is critical for CFD hot-path calculations.
//  * - Cache Locality: Contiguous stack storage improves CPU cache utilization and enables better optimization by the compiler.
// Calculates discrete scheme coefficients based on diffusion number (r)
[[nodiscard]] inline std::array<double, 2> ReturnCoeffs(double DiffNumber_) noexcept
{
    const double r = DiffNumber_;
    const double denom = 1.0 + 2.0 * r;

    const double C_Old = (1.0 - 2.0 * r) / denom;
    const double C     = (2.0 * r) / denom;

    return {C_Old, C};
}


}

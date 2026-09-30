#include "Discretization/Explicit/DuFortFrankel/DuFortFrankel.hpp"

#include <array>

// =====================
// DUFORT_FRANKEL Scheme
// ====================================================================================
//EQ : U_i^(n+1) = [(1 - 2r) / (1 + 2r)] U_i^(n-1)+ [2r / (1 + 2r)] (U_(i-1)^n + U_(i+1)^n)
//where r is the diffusion number
// =====================================================================================

namespace Discretization::Explicit::DuFortFrankel
{
    // Updates Field from time levels (n-1, n) to n+1
    void Step(
        const Field& Field_nMinus1,
        const Field& Field_n,
        Field& Field_nPlus1,
        std::array<double, 2> Coeffs
    ) noexcept
    {
        std::size_t N = Field_n.GetSize();
        for (std::size_t i = 1; i < N - 1; ++i)
        {
            double RHS = Coeffs[0] * Field_nMinus1[i]
                + Coeffs[1] * (Field_n[i + 1] + Field_n[i - 1]);
            Field_nPlus1[i] = RHS;
        }
    }
}
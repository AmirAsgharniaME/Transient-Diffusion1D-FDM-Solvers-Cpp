#include "Discretization/Explicit/FTCS/FTCS.hpp"

#include <cstddef>

// ===========================================================================================
// FTCS Scheme
// ===========================================================================================
//EQ : U_i^(n+1) = U_i^n + r (U_(i+1)^n - 2 U_i^n + U_(i-1)^n)
//where r is the diffusion number
// ============================================================================================


namespace Discretization::Explicit::FTCS
{
void Step(
    const Field& Field_n,
    Field& Field_nPlus1,
    double DiffNumber
) noexcept
{
    std::size_t N = Field_n.GetSize();
    //Internal Nodes
    for (std::size_t i = 1; i < N - 1; ++i)
    {
        double RHS = Field_n[i]+ DiffNumber * (Field_n[i + 1]- 2.0 * Field_n[i]+ Field_n[i - 1]);
        Field_nPlus1[i] = RHS;
    }
}

} // namespace Discretization::Explicit::FTCS

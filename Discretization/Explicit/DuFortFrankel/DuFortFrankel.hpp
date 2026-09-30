#pragma once

#include <array>
#include "Core/Field/Field.hpp"

namespace Discretization::Explicit::DuFortFrankel
{
    [[nodiscard]] inline std::array<double, 2> CalculateCoefficients(double DiffNumber) noexcept
    {
        const double denom = 1.0 + 2.0 * DiffNumber;
        const double C_Old = (1.0 - 2.0 * DiffNumber) / denom;
        const double C     = (2.0 * DiffNumber) / denom;
        return {C_Old, C};
    }

    // Updates Field from time levels (n-1, n) to n+1
    void Step(
        const Field& Field_nMinus1,
        const Field& Field_n,
        Field& Field_nPlus1,
        std::array<double, 2> Coeffs
    ) noexcept;
}

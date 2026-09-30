#pragma once

#include <cassert>

/**
 * @brief Computes the 1D diffusion characteristic time scale for a rod or slab.
 * 
 * Formula: t_scale = (Lx^2) / alpha
 * 
 * @param Lx Physical length of the domain in X direction [m]
 * @param alpha Thermal diffusivity of the material [m^2 / s]
 * @return constexpr double The physical time scale (t_scale) [s]
 */

 namespace TimeScale
{
    [[nodiscard]] constexpr double Return(double Lx, double nu) noexcept
    {
        assert(Lx > 0.0 && "Lx must be strictly positive");
        assert(nu > 0.0 && "Thermal diffusivity (alpha) must be strictly positive");

        return (Lx * Lx) / nu;
    }
}

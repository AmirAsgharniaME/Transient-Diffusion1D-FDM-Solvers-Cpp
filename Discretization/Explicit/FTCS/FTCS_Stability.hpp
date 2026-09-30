#pragma once

#include <cmath>
#include <iostream>
#include <stdexcept>
#include "SimulationSetup/SimulationParams/SimulationParams.hpp"


namespace Discretization::Explicit::FTCS
{

/**
 * @brief Checks FTCS stability and reduces dt when the scheme is unstable.
 *
 * The one-dimensional FTCS stability condition is
 *      DiffNumber = (nu * dt) / (dy^2) <= 0.5.
 * The stable time step is selected as
 *      dt_new = safetyFactor * (dy^2) / (2.0 * nu).
 */
inline void Stabilize(
    SimulationParams& params,
    double safetyFactor = 0.95
)
{
    constexpr double StabilityLimit = 0.5;
    const double currentDiffNumber = params.DiffNumber.GetValue();

    if (std::isnan(currentDiffNumber) || currentDiffNumber < 0.0)
    {
        throw std::invalid_argument(
            "FTCS stability: diffusion number must be nonnegative and not NaN."
        );
    }

    if (currentDiffNumber < StabilityLimit)
    {
        std::cout << "[INFO] FTCS Scheme is STABLE. Current Diffusion Number = "
                  << currentDiffNumber << '\n';
        return;
    }

    const double currentDt = params.dt.GetValue();
    const double dy = params.Delta.GetValue();
    const double nu = params.nu.GetValue();

    std::cout << "[WARNING] Diffusion Number limit is 0.5 but Current Diffusion Number = "
              << currentDiffNumber << '\n';
    std::cout << "[WARNING] FTCS is unstable with dt = " << currentDt << '\n';

    if (!std::isfinite(safetyFactor)
        || safetyFactor <= 0.0
        || safetyFactor > 1.0)
    {
        throw std::invalid_argument(
            "FTCS stability: safetyFactor must be in the range (0.0, 1.0]."
        );
    }
    if (!std::isfinite(currentDt) || currentDt <= 0.0)
    {
        throw std::invalid_argument(
            "FTCS stability: current dt must be positive and finite."
        );
    }
    if (!std::isfinite(dy) || dy <= 0.0)
    {
        throw std::invalid_argument(
            "FTCS stability: mesh spacing must be positive and finite."
        );
    }
    if (!std::isfinite(nu) || nu <= 0.0)
    {
        throw std::invalid_argument(
            "FTCS stability: diffusivity must be positive and finite."
        );
    }

    const double newDt = safetyFactor * (dy * dy) / (2.0 * nu);
    if (!std::isfinite(newDt) || newDt <= 0.0)
    {
        throw std::overflow_error(
            "FTCS stability: calculated stable dt is not representable."
        );
    }

    const double newDiffNumber = (nu * newDt) / (dy * dy);
    if (!std::isfinite(newDiffNumber) || newDiffNumber < 0.0)
    {
        throw std::overflow_error(
            "FTCS stability: calculated diffusion number is not representable."
        );
    }

    params.dt.SetValue(newDt);
    params.DiffNumber.SetValue(newDiffNumber);

    std::cout << "[FIX] dt has been reduced for FTCS stability.\n";
    std::cout << "      New dt = " << newDt << '\n';
    std::cout << "      New Diffusion Number = " << newDiffNumber << '\n';
}

} // namespace Discretization::Explicit::FTCS

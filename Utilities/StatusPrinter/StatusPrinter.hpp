#pragma once

#include <iostream>
#include <iomanip>
#include <cstddef>

namespace StatusPrinter 
{

    inline void PrintStepStatus(std::size_t TimeLevel, double Residual, double dt_) noexcept 
    {
        const double Time = static_cast<double>(TimeLevel) * dt_;
        std::cout << "Step: " << std::setw(6) << TimeLevel
                  << " | Time: " << std::fixed << std::setprecision(2)
                  << std::setw(10) << Time << " s"
                  << " | Relative residual: " << std::scientific
                  << std::setprecision(2) << Residual
                  << std::defaultfloat << '\n';
    }

    inline void PrintConvergenceStatus(std::size_t TimeLevel, double Residual, double dt_) noexcept
     {
        const double Time = static_cast<double>(TimeLevel) * dt_;
        std::cout << "\n"
                  << "========================================\n"
                  << "Convergence achieved\n"
                  << "Step   : " << std::setw(6) << TimeLevel << '\n'
                  << "Time   : " << std::fixed << std::setprecision(2)
                  << std::setw(10) << Time << " s\n"
                  << "Residual: " << std::scientific << std::setprecision(2) << Residual << '\n'
                  << "========================================\n"
                  << std::defaultfloat;
    }
}


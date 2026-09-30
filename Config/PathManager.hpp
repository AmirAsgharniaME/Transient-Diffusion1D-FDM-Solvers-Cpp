#pragma once

#include <filesystem>
#include "PathType.hpp"
#include "SolverScheme.hpp"


namespace PathManager
{
    // Generates a resolved relative directory path based on category and numerical scheme
    [[nodiscard]] inline std::filesystem::path GetOutputPath(OutputCategory category, SolverScheme scheme = SolverScheme::FTCS)
    {
        namespace fs = std::filesystem;

        // Map category to its respective root directory
        switch (category)
        {
            case OutputCategory::Initial:
                return fs::path("Results/Initial_Field1D");
                
            case OutputCategory::Numerical:
                return fs::path("Results/Numerical_Field1D") / To_String(scheme);

            case OutputCategory::Analytical:
                return "Results/Analytical_Field1D";

            default:
                return "Results/Unknown";
        }
    }
}


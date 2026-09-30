#pragma once
#include <vector>
#include <cstddef>

namespace SolverInputs 
{

    namespace Geometry 
    {
        inline constexpr double Length = 0.04; //[m]
    }

    namespace Physics 
    {
        inline constexpr double nu = 0.00217;
    }

    namespace Solver 
    {
        inline constexpr std::size_t NumTimeLevels = 20000;
        inline constexpr double dt = 0.0001;
        inline constexpr double Tolerance = 1e-6;
    }

    namespace Mesh 
    {
        inline constexpr std::size_t N = 40;
    }

    namespace InitialCondition 
    {
        inline const std::vector<double> Profile(Mesh::N, 0.0);
    }

    namespace BoundaryCondition 
    {
        inline constexpr double TopWallValue = 0.0;
        inline constexpr double BottomWallValue = 40.0;
    }

} // namespace SolverInputs

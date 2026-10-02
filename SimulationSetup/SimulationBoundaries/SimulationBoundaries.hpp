#pragma once
#include "Core/Boundary/Boundary.hpp"
#include "Config/SolverInputs.hpp"
#include "Core/Boundary/BoundaryManager/BoundaryManager.hpp"
class SimulationBoundaries
{
public:
    Boundary TopWall;
    Boundary BottomWall;
    BoundaryType GetType() const noexcept
    {
        return Type;
    }

    explicit SimulationBoundaries()
        : TopWall(
              BoundaryType::Dirichlet,
              Orientation::Vertical,
              BoundaryLocation::Top,
              SolverInputs::BoundaryCondition::TopWallValue
          ),
          BottomWall(
              BoundaryType::Dirichlet,
              Orientation::Vertical,
              BoundaryLocation::Bottom,
              SolverInputs::BoundaryCondition::BottomWallValue
          ),
          Type(BoundaryType::Dirichlet)
    {}

    private:
    BoundaryType Type;
};
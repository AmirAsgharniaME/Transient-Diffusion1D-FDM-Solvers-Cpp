#include "SimulationSetup/SimulationBoundaries/SimulationBoundaries.hpp"

#include "Config/SolverInputs.hpp"

SimulationBoundaries::SimulationBoundaries()
    : TopWall(
          BoundaryType::Dirichlet,
          Orientation::Vertical,
          BoundaryLocation::Top,
          SolverInputs::BoundaryCondition::TopWallValue),
      BottomWall(
          BoundaryType::Dirichlet,
          Orientation::Vertical,
          BoundaryLocation::Bottom,
          SolverInputs::BoundaryCondition::BottomWallValue),
      Type(BoundaryType::Dirichlet)
{}

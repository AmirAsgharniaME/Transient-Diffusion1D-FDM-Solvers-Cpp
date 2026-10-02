#include "SimulationSetup/SimulationParams/SimulationParams.hpp"

#include "Config/SolverInputs.hpp"
#include "Utilities/Convergence/RelativeFieldChange/TimeScale/TimeScale.hpp"

SimulationParams::SimulationParams(const Mesh& grid)
    : nu(SolverInputs::Physics::nu),
      NumTimeLevels(SolverInputs::Solver::NumTimeLevels),
      dt(SolverInputs::Solver::dt),
      Tolerance(SolverInputs::Solver::Tolerance),
      Delta(grid.Get_Delta()),
      DiffNumber((nu.GetValue() * dt.GetValue()) /
                 (Delta.GetValue() * Delta.GetValue())),
      t_Scale(TimeScale::Return(Delta.GetValue(), nu.GetValue()))
{}

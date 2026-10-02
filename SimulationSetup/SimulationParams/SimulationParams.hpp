#pragma once

#include "Utilities/Parameter/Parameter.hpp"
#include "Core/Mesh/Mesh.hpp"
#include "Config/SolverInputs.hpp"
#include "Utilities/Convergence/RelativeFieldChange/TimeScale/TimeScale.hpp"
class SimulationParams
{
public:
    Parameter<double> nu;
    Parameter<std::size_t> NumTimeLevels;
    Parameter<double> dt;
    Parameter<double> Tolerance;
    Parameter<double> Delta;
    Parameter<double> DiffNumber;
    Parameter<double> t_Scale;
   
   
    

    explicit SimulationParams(const Mesh& grid)
    : nu(SolverInputs::Physics::nu),
      NumTimeLevels(SolverInputs::Solver::NumTimeLevels),
      dt(SolverInputs::Solver::dt),
      Tolerance(SolverInputs::Solver::Tolerance),
      Delta(grid.Get_Delta()),
      DiffNumber((nu.GetValue() * dt.GetValue()) / (Delta.GetValue() * Delta.GetValue())),
      t_Scale(TimeScale::Return(Delta.GetValue(), nu.GetValue()))
      {}
};
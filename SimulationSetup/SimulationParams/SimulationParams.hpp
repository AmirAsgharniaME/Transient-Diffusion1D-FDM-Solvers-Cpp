#pragma once

#include "Utilities/Parameter/Parameter.hpp"
#include "Core/Mesh/Mesh.hpp"

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
   
   
    

    explicit SimulationParams(const Mesh& grid);
};

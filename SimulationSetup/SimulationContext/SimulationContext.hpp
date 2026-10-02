#pragma once


#include "Core/Field/Field.hpp"
#include "SimulationSetup/SimulationParams/SimulationParams.hpp"
#include "SimulationSetup/SimulationBoundaries/SimulationBoundaries.hpp"
#include "Discretization/SolverScheme/SolverScheme.hpp"
#include "SimulationSetup/PostProcessing/Plot/Plot.hpp"


class SimulationContext
{
public:
    Geometry geometry;
    Mesh grid;
    SimulationParams params;
    SimulationBoundaries boundaries;
    Plot plot;

    Field U_0;
    Field U_1;
    Field U_nMinus1;
    Field U_n;
    Field U_nPlus1;
    Field U_SteadyState;
    Field U_Analytical_n;

    explicit SimulationContext(SolverScheme scheme_);
    
 
    void StartUp();
    void Common(std::size_t TimeLevel_n);
    bool GetBreakCondition();
    void WriteInitialOutputs();

    private:
    SolverScheme scheme;
    double RelativeFieldChange;
    double FinalTime;
    bool BreakCondition;
    void SetRelativeFieldChange(double value);
    void SetFinalTime(double value);
    void SetBreakCondition(bool value);
    void ApplyBC();
    double CalculateRelativeFieldChange();
    void PrintStatus(std::size_t TimeLevel_n);
    void UpdadeField();
    void LivePlot(std::size_t TimeLevel_n);
    void WriteToFileTimeLevel(std::size_t TimeLevel_n);
    void PlotTimeLevel(std::size_t TimeLevel_n);
    bool IsSteadyState(std::size_t TimeLevel_n);
};

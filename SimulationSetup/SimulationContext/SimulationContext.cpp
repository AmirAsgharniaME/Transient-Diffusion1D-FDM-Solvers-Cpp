#include "SimulationSetup/SimulationContext/SimulationContext.hpp"

#include "IO/OutputWriter/FileWriter/FileWriter.hpp"
#include "IO/OutputWriter/FileName/FileName.hpp"
#include "IO/PathManager/PathManager/PathManager.hpp"
#include "IO/Visualization/Legend/Legend.hpp"
#include "Verification/AnalyticalSolution/AnalyticalSolution.hpp"
#include "Config/SolverInputs.hpp"
#include "Utilities/Convergence/RelativeFieldChange/FieldChange.hpp"
#include "Utilities/StatusPrinter/StatusPrinter.hpp"
#include "Verification/AnalyticalSolution/AnalyticalSolution.hpp"

SimulationContext::SimulationContext(SolverScheme scheme_)
    : geometry(SolverInputs::Geometry::Length),
      grid(geometry, SolverInputs::Mesh::N),
      params(grid),
      boundaries(),
      U_0(grid),
      U_1(grid),
      U_nMinus1(grid),
      U_n(grid),
      U_nPlus1(grid),
      U_SteadyState(grid),
      U_Analytical_n(grid),
      scheme(scheme_),
      RelativeFieldChange(0.0),
      FinalTime(0.0),
      BreakCondition(false)
{
    U_0.SetIntitialProfile(SolverInputs::InitialCondition::Profile);
    U_0.ApplyBoundaryCondition(boundaries.TopWall);
    U_0.ApplyBoundaryCondition(boundaries.BottomWall);

    DiffusionEQ::AnalyticalSolution::Pass(
        U_SteadyState,
        geometry,
        grid,
        boundaries
    );
}





void SimulationContext::Common(std::size_t TimeLevel_n)
{

    ApplyBC();
    double Value = CalculateRelativeFieldChange();
    SetRelativeFieldChange(Value);
    PrintStatus(TimeLevel_n);
    UpdadeField();

    LivePlot(TimeLevel_n);
    WriteToFileTimeLevel(TimeLevel_n);
    PlotTimeLevel(TimeLevel_n);

    if(IsSteadyState(TimeLevel_n))
    {
        return;
    }

    
}
 bool SimulationContext::GetBreakCondition()
 {
    return BreakCondition;
 }

 void SimulationContext::WriteInitialOutputs()
{
    FileWriter::WriteField(
        FileName::Create(FieldName::U, 0.0),
        U_0,
        grid,
        Path::Create(OutputCategory::Initial)
    );

    FileWriter::WriteField(
        FileName::Create(FieldName::UAnalytical, Label::Steady_State),
        U_SteadyState,
        grid,
        Path::Create(OutputCategory::Analytical)
    );

    plot.Window.AddPlot(
        Path::Create(OutputCategory::Initial),
        FileName::Create(FieldName::U,0.0),
        Plotter::LineStyle::Solid,
        2.5, 
        Legend::Create(FieldName::U,0.0));

        plot.Window.AddPlot(
        Path::Create(OutputCategory::Analytical),
        FileName::Create(FieldName::UAnalytical,Label::Steady_State),
        Plotter::LineStyle::DashDotted,
        2.5,
        Legend::Create(FieldName::UAnalytical,Label::Steady_State));

}

//Private Mthods
void SimulationContext::SetRelativeFieldChange(double value)
{
    RelativeFieldChange = value;
}
void SimulationContext::SetFinalTime(double value)
{
    FinalTime = value;
}
void SimulationContext::SetBreakCondition(bool value)
{
    BreakCondition = value;
}

void SimulationContext::StartUp()
{
    if (scheme == SolverScheme::FTCS 
        || scheme == SolverScheme::Laasonen 
        || scheme == SolverScheme::CrankNicolson)
    {
            U_n.Swap(U_0);
    }
    else if (scheme == SolverScheme::DUFORT_FRANKEL)
    {
        return;
    }
}

void SimulationContext::ApplyBC()
 {
    //Apply Boundary Conditions To Field
    U_nPlus1.ApplyBoundaryCondition(boundaries.TopWall);
    U_nPlus1.ApplyBoundaryCondition(boundaries.BottomWall);
 }

 double SimulationContext::CalculateRelativeFieldChange()
 {
    //Calculate The Relative Field Change from U_n to U_nPlus1
    double value = RelativeFieldChange::Return(
        U_n,
        U_nPlus1,
        params.dt.GetValue(),
        params.t_Scale.GetValue()
    );
    return value;
 }

 void SimulationContext::PrintStatus(std::size_t TimeLevel_n)
 {
        StatusPrinter::Print(
        "Relative Field Change",
        TimeLevel_n,
        RelativeFieldChange,
        params.dt.GetValue());
 }

  void SimulationContext::UpdadeField()
  {
    if (scheme == SolverScheme::FTCS || scheme == SolverScheme::Laasonen || scheme == SolverScheme::CrankNicolson)
    {     //Update Field
        U_n.Swap(U_nPlus1);
    }
    else if (scheme == SolverScheme::DUFORT_FRANKEL)
    {
        U_nMinus1.Swap(U_n);
        U_n.Swap(U_nPlus1);
    }
  }

void SimulationContext::LivePlot(std::size_t TimeLevel_n)
{
if (TimeLevel_n % 20 == 0)
    {
        plot.LiveWindow.AddPoint(
            "relative field change",
            TimeLevel_n,
            RelativeFieldChange);
    }
}

 void SimulationContext::WriteToFileTimeLevel(std::size_t TimeLevel_n)
 {
if(TimeLevel_n % 200 == 0)
    {
        FileWriter::WriteField
        (
        FileName::Create(FieldName::U,static_cast<double>(TimeLevel_n)*params.dt.GetValue()),
        U_n,
        grid,
        Path::Create(OutputCategory::Numerical, scheme)
        );

    }

 }

 void SimulationContext::PlotTimeLevel(std::size_t TimeLevel_n)
 {
    if (TimeLevel_n % 200 == 0)
    {
            plot.Window.AddPlot(
            Path::Create(OutputCategory::Numerical, scheme), 
            FileName::Create(FieldName::U,static_cast<double>(TimeLevel_n)*params.dt.GetValue()), 
            Plotter::LineStyle::Solid,
            2.5, 
            Legend::Create(FieldName::U,static_cast<double>(TimeLevel_n)*params.dt.GetValue())
        );
    }
 }

 bool SimulationContext::IsSteadyState(std::size_t TimeLevel_n)
 {
    if(RelativeFieldChange < params.Tolerance.GetValue())
    {
        StatusPrinter::Print(
            "Relative Field Change",
            TimeLevel_n,
            RelativeFieldChange,
            params.dt.GetValue(),true);

        double finaltime = static_cast<double>(TimeLevel_n) * params.dt.GetValue();
        SetFinalTime(finaltime);

        FileWriter::WriteField(
            FileName::Create(FieldName::U,FinalTime),
            U_n,
            grid,
            Path::Create(OutputCategory::Numerical, scheme)
            );

        plot.Window.AddPlot(
            Path::Create(OutputCategory::Numerical, scheme),
            FileName::Create(FieldName::U,FinalTime),
            Plotter::LineStyle::Dashed,
            2.0,
            Legend::Create(FieldName::U,FinalTime)
            );
       
        DiffusionEQ::AnalyticalSolution::Pass(
            U_Analytical_n,
            FinalTime,
            params.nu.GetValue(),
            geometry,
            grid,
            boundaries,
            U_n);


        FileWriter::WriteField(
            FileName::Create(FieldName::UAnalytical,FinalTime),
            U_Analytical_n,
            grid,
            Path::Create(OutputCategory::Analytical));

        plot.Window.AddPlot(
            Path::Create(OutputCategory::Analytical),
            FileName::Create(FieldName::UAnalytical,FinalTime),
            Plotter::LineStyle::DashDotted,
            2.5,
            Legend::Create(FieldName::UAnalytical,FinalTime)
        );
        std::cout << "Calculations Completed Successfully for " << std::string(To_String(scheme)) << '\n';
        SetBreakCondition(true);
        return true;
    }
    else
    {
        SetBreakCondition(false);
        return false;
    }
 }

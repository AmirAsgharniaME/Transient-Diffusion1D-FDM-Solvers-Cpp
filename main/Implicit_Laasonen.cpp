
// ===========
// C++ Libraries
// ===========
#include <string>
#include <iostream>
// #include <vector>
// #include <cmath> 
// #include <iomanip>
// #include <thread> // Added for optional small delays



// ========
// Config
// ======== 
#include "Config/SolverScheme.hpp"

// ========
// Utilities
// ======== 

#include "Utilities/KeyboardHandler/KeyboardHandler.hpp"
#include "Utilities/StatusPrinter/StatusPrinter.hpp"

// ========
// Core1D
// ======== 
#include "Core1D/Convergence1D/Convergence1D.hpp"


// =====================
// Implicit Solvers 1D
// =====================
#include "ImplicitSolvers1D/CoefficientMatrix1D/CoefficientMatrix1D.hpp"
#include "ImplicitSolvers1D/RHS1D/RHS1D.hpp"
#include "ImplicitSolvers1D/TridiagonalMatrix/TridiagonalMatrix.hpp"

//==============
//Linear Solvers
//==============
#include "LinearSolvers/GaussianElimination/GaussianElimination.hpp"
#include "LinearSolvers/ThomasAlgorithm/ThomasAlgorithm.hpp"

//Initial Setup
#include "Core1D/Initial_Setup1D.hpp"

//Plotter Setup
#include "Core1D/Plotter_Setup1D.hpp"


int main() 
{
//-------------------------------------------------------------------------------------------------
 Initial_Setup1D Setup;
 Plotter_Setup1D PlotterSetup;
//-----------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//Start Up
Setup.U_n.Swap(Setup.U_0);
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//Create Coefficient Matrix For Gaussian Elimination
CoefficientMatrix1D A(SolverScheme::Laasonen,Setup.mesh1D,Setup.DiffNumber.GetValue());
A.SetBoundaryConditions(); //Drichlet BC
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//Create Tridiagonal Matrix for Thomas Algorithm
TridiagonalMatrix A3(SolverScheme::Laasonen,Setup.mesh1D,Setup.DiffNumber.GetValue());
A3.SetBoundaryConditions(); //Drichlet BC
//-------------------------------------------------------------------------------------------------

//Create RHS
// RHS1D RHS1D(SolverScheme::Laasonen,Setup.U_n,Setup.DiffNumber.GetValue());
// RHS1D.ApplyBoundaryCondition(Setup.U_0);


// =============================================================================
//2-SolverSetup:
// =============================================================================
//1.Live Plotter Initialization
//2.Defining Loop Parameters
//3.main Solver Loop
//3.1. A Condition that Stops The Loop Solver Iterations by Pressing ESC
//3.2. Linear Solver For Laasonen
//3.3. Apply Boundary Conditions To Field1D
//3.4. Claculate The Relative Field Change from U_n to U_nPlus1
//3.5. Print Step Status
//3.6. Update
//3.7. Live Plotter
//3.8. Codition For Convergence To stady state Solution
// =============================================================================
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//Defining Loop Parameters
std::size_t TotalTimeLevel = Setup.NumTimeLevels.GetValue();
double dt_ = Setup.dt.GetValue();
double t_Scale = (Setup.Length.GetLength()*Setup.Length.GetLength()) / (Setup.nu.GetValue());
double DiffNumber_ = Setup.DiffNumber.GetValue();
//------------------------------------------------------------------------------
//main Solver Loop
for(std::size_t TimeLevel_n = 1; TimeLevel_n <= TotalTimeLevel; TimeLevel_n++){
//------------------------------------------------------------------------------Start Solver Loop

//This Condition Stops The Loop Solver Iterations by Pressing ESC
if (isEscPressed())
{
    std::cout << "\nESC pressed. Exiting program now..." << std::endl;
    return 0;
}

//Linear Solver For Laasonen

//Gaussian Elimination Setup
// CoefficientMatrix1D A_Copy = A;
// RHS1D RHS1D(SolverScheme::Laasonen,Setup.U_n,DiffNumber_);
// RHS1D.ApplyBoundaryCondition(Setup.U_n);
// GaussianElimination::Solve_nPlus1(A_Copy,RHS1D,Setup.U_nPlus1);

//Thomas Algorithm Setup
TridiagonalMatrix A3_Copy = A3;
RHS1D RHS1D(SolverScheme::Laasonen,Setup.U_n,DiffNumber_);
RHS1D.ApplyBoundaryCondition(Setup.U_n);
ThomasAlgorithm::Solve_nPlus1(A3_Copy,RHS1D,Setup.U_nPlus1);


//Apply Boundary Conditions To Field1D
Setup.U_nPlus1.ApplyBoundaryCondition(BoundaryLocation::Top,Setup.TopWall);
Setup.U_nPlus1.ApplyBoundaryCondition(BoundaryLocation::Bottom,Setup.BottomWall);


//Claculate The Relative Field Change from U_n to U_nPlus1
double RateOfFieldChange = RelativeFieldChange1D::ReturnFieldChange(
    Setup.U_n,
    Setup.U_nPlus1,
     dt_,
     t_Scale);
StatusPrinter::PrintStepStatus(TimeLevel_n,RateOfFieldChange, dt_);

//Update
Setup.U_n.Swap(Setup.U_nPlus1);

//Live Plotter
if (TimeLevel_n % 20 == 0) 
{
PlotterSetup.LiveWindow.AddPoints(
static_cast<double>(TimeLevel_n),
{
    {"rate_of_field_change", RateOfFieldChange}
});

}

if(TimeLevel_n % 300 == 0)
{
    FileWriter1D::WriteField1D
    (
        NameCreator::CreateName(FieldName::U,static_cast<double>(TimeLevel_n)*dt_),
        Setup.U_n,
        Setup.mesh1D,
        PathManager::GetOutputPath(OutputCategory::Numerical, SolverScheme::Laasonen)
    );

    PlotterSetup.Window.AddPlot
    (
        PathManager::GetOutputPath(OutputCategory::Numerical, SolverScheme::Laasonen),
        NameCreator::CreateName(FieldName::U,static_cast<double>(TimeLevel_n)*dt_),
        OutputPlotter1D::LineStyle::Dashed,
        2.0,
        LegendCreator::CreatePlotLegend(FieldName::U,static_cast<double>(TimeLevel_n)*dt_)
    );
   
}

//Codition For Convergence To stady state Solution
if(RateOfFieldChange < Setup.Tolerance.GetValue())
{
StatusPrinter::PrintConvergenceStatus(TimeLevel_n,RateOfFieldChange,dt_);
double FinalTime = static_cast<double>(TimeLevel_n) * dt_;

FileWriter1D::WriteField1D
    (
    NameCreator::CreateName(FieldName::U,FinalTime),
    Setup.U_n,
    Setup.mesh1D,
    PathManager::GetOutputPath(OutputCategory::Numerical, SolverScheme::Laasonen)
    );

PlotterSetup.Window.AddPlot
    (
    PathManager::GetOutputPath(OutputCategory::Numerical, SolverScheme::Laasonen),
    NameCreator::CreateName(FieldName::U,FinalTime),
    OutputPlotter1D::LineStyle::Dashed,
    2.0,
    LegendCreator::CreatePlotLegend(FieldName::U,FinalTime)
    );
   


AnalyticalDiffusion1D U_Analytical_1D_n
    (
    FinalTime,
     Setup.mesh1D,
     Setup.Length,
     Setup.TopWall,
    Setup.BottomWall
    );

FileWriter1D::WriteField1D(
    NameCreator::CreateName(FieldName::UAnalytical,FinalTime),
    U_Analytical_1D_n,
    Setup.mesh1D,
    PathManager::GetOutputPath(OutputCategory::Analytical));

PlotterSetup.Window.AddPlot(
    PathManager::GetOutputPath(OutputCategory::Analytical),
    NameCreator::CreateName(FieldName::UAnalytical,FinalTime),
    OutputPlotter1D::LineStyle::DashDotted,
    2.5,
    LegendCreator::CreatePlotLegend(FieldName::UAnalytical,FinalTime)
);

    break;
}


//----------------------------------------------------------------------------End Solver Loop
}
 PlotterSetup.LiveWindow.Close();
//----------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------------------------------
    std::cout << "Calculations Completed Successfully for " << std::string(To_String(SolverScheme::Laasonen)) << std::endl;
    std::cin.get();
    return 0;
}
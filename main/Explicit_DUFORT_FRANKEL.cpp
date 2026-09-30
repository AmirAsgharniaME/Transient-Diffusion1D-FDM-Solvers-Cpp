// ===========
// C++ Libraries
// ===========
#include <iostream>
#include <cmath>
#include <array>
// #include <vector>

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


// =========
// Stability
// ========= 
#include "Stability/FTCS/Stabilization.hpp"


// ===========
// ExplicitSolvers1D
// ===========
#include "ExplicitSolvers1D/ExplicitSolvers1D.hpp"


//Initial Setup
#include "Core1D/Initial_Setup1D.hpp"
#include "Core1D/Plotter_Setup1D.hpp"

int main() 
{
//-------------------------------------------------------------------------------------------------
 Initial_Setup1D Setup;
 Plotter_Setup1D PlotterSetup;
//-----------------------------------------------------------------------------------------------

Field1D U_1(Setup.Length,Setup.mesh1D);
Field1D U_nMinus1(Setup.Length,Setup.mesh1D);

//Start Up
//===========================================
//FTCS is a StartUp for DuFortFrankel Method
//U_0----->Explicit FTCS------> U_1
//===========================================

if (Setup.DiffNumber.GetValue() < 0.5)
{
    std::cout<<"FTCS Scheme is Stable For StartUp"<<'\n';
    FTCS::Solve_nPlus1(Setup.U_0,U_1,Setup.DiffNumber.GetValue());
    //Apply Boundary Conditions To New Field
    U_1.ApplyBoundaryCondition(BoundaryLocation::Top,Setup.TopWall);
    U_1.ApplyBoundaryCondition(BoundaryLocation::Bottom,Setup.BottomWall);
    U_nMinus1.Swap(Setup.U_0);
    Setup.U_n.Swap(U_1);
}

    StabilityParams StabPrams = 
    {
        &Setup.dt,
        &Setup.DiffNumber,
        &Setup.nu
    };

if (Setup.DiffNumber.GetValue() >= 0.5)
{
    double Original_DiffNumber = Setup.DiffNumber.GetValue();
    double Original_dt = Setup.dt.GetValue();

    std::cout<<"FTCS Scheme is Unstable For StartUp"<<'\n';
    Stabilization(Setup.mesh1D,StabPrams);
    double Lower_DiffNumber = Setup.DiffNumber.GetValue();
    double Lower_dt = Setup.dt.GetValue();
    std::size_t n = static_cast<std::size_t>(std::llround(Original_dt/ Lower_dt));

    Setup.dt.SetValue(Original_dt);
    Setup.DiffNumber.SetValue(Original_DiffNumber);


    
    U_nMinus1=Setup.U_0; //Saving U_n-1 = U_0

    // U_0 ----> U_1 with Lower_dt and Lower_DiffNumber
    for (size_t i = 1; i <= n; i++)
    {
        FTCS::Solve_nPlus1(Setup.U_0 , U_1 , Lower_DiffNumber);

        //Apply Boundary Conditions To New Field
        U_1.ApplyBoundaryCondition(BoundaryLocation::Top,Setup.TopWall);
        U_1.ApplyBoundaryCondition(BoundaryLocation::Bottom,Setup.BottomWall);

        if (i==n)
        {
            //U_nPlus1 = U_1
            Setup.U_n.Swap(U_1);
            break;
        }
        //update U_0
        Setup.U_0.Swap(U_1);
    }



}// End Of if (DiffusionNumber >= 0.5)
//============================================================================
// At this Time U_n = U_1 and U_nMinus1 = U_0 and U_nPlus1 is empty
//============================================================================



// ==========================
// Solver Loop 
// ===========================
//Defining Loop Parameters
std::size_t TotalTimeLevel = Setup.NumTimeLevels.GetValue();
double dt_ = Setup.dt.GetValue();
double t_Scale = (Setup.Length.GetLength()*Setup.Length.GetLength()) / (Setup.nu.GetValue());

// Coffes of Discrete Algebraic Equation For DuFortFrankel Scheme
std::array<double, 2> Coeffs = DUFORT_FRANKEL::ReturnCoeffs(Setup.DiffNumber.GetValue());
//------------------------------------------------------------------------------
//main Solver Loop
for(std::size_t TimeLevel_n = 2; TimeLevel_n <= TotalTimeLevel; TimeLevel_n++){
//------------------------------------------------------------------------------Start Solver Loop

//This Condition Stops The Loop Solver Iterations by Pressing ESC
if (isEscPressed())
{
    std::cout << "\nESC pressed. Exiting program now..." << std::endl;
    return 0;
}

DUFORT_FRANKEL::Solve_nPlus1(
    Setup.U_n, 
    U_nMinus1, 
    Setup.U_nPlus1, 
    Coeffs);

    //Apply Boundary Conditions To New Field
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
    U_nMinus1.Swap(Setup.U_n);
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
        PathManager::GetOutputPath(OutputCategory::Numerical, SolverScheme::DUFORT_FRANKEL)
    );

    PlotterSetup.Window.AddPlot
    (
        PathManager::GetOutputPath(OutputCategory::Numerical, SolverScheme::DUFORT_FRANKEL),
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
    PathManager::GetOutputPath(OutputCategory::Numerical, SolverScheme::DUFORT_FRANKEL)
    );

PlotterSetup.Window.AddPlot
    (
    PathManager::GetOutputPath(OutputCategory::Numerical, SolverScheme::DUFORT_FRANKEL),
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



//-----------------------------------------------------------------------------------------------------------------------------
    std::cout << "Calculations Completed Successfully for " << std::string(To_String(SolverScheme::DUFORT_FRANKEL)) << std::endl;
    std::cin.get();
    return 0;
}//Ebd of main
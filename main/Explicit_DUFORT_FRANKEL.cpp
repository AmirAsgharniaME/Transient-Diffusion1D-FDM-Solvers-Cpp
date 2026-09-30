// C++ Libraries
#include <iostream>
#include <cmath>
//Local Files
#include "Discretization/Explicit/FTCS/FTCS.hpp"
#include "Discretization/SolverScheme/SolverScheme.hpp"
#include "SimulationSetup/SimulationContext/SimulationContext.hpp"
#include "Utilities/KeyboardHandler/KeyboardHandler.hpp"
#include "Discretization/Explicit/FTCS/FTCS.hpp"
#include "Discretization/Explicit/FTCS/FTCS_Stability.hpp"
#include "Discretization/Explicit/DuFortFrankel/DuFortFrankel.hpp"


int main() 
{
//-------------------------------------------------------------------------------------------------
//Simulation Setup
SimulationContext ctx(SolverScheme::DUFORT_FRANKEL);
//-----------------------------------------------------------------------------------------------


//Stabilization and Time Step

ctx.StartUp(); //U_n = U_1 and U_nMinus1 = U_0

//Start Up
if (ctx.params.DiffNumber.GetValue() < 0.5)
{
std::cout<<"FTCS Scheme is Stable For StartUp"<<'\n';
Discretization::Explicit::FTCS::Step(
ctx.U_0, 
ctx.U_1, 
ctx.params.DiffNumber.GetValue());
//Apply Boundary Conditions To New Field
ctx.U_1.ApplyBoundaryCondition(ctx.boundaries.TopWall);
ctx.U_1.ApplyBoundaryCondition(ctx.boundaries.BottomWall);
ctx.U_nMinus1.Swap(ctx.U_0);
ctx.U_n.Swap(ctx.U_1);
}


if (ctx.params.DiffNumber.GetValue() >= 0.5)
{
    double Original_DiffNumber = ctx.params.DiffNumber.GetValue();
    double Original_dt = ctx.params.dt.GetValue(); 
    //Stabilization
    Discretization::Explicit::FTCS::Stabilize(ctx.params);
    double Lower_DiffNumber = ctx.params.DiffNumber.GetValue();
    double Lower_dt = ctx.params.dt.GetValue();
    //Calculating the substeps to get from U0 to U1
    std::size_t n = static_cast<std::size_t>(std::llround(Original_dt/ Lower_dt));
    //Restore the Original Values to Params
    ctx.params.dt.SetValue(Original_dt);
    ctx.params.DiffNumber.SetValue(Original_DiffNumber);

    //Saving U_n-1 = U_0
    ctx.U_nMinus1 = ctx.U_0;
    for (size_t i = 1; i <= n; i++)
    {
        Discretization::Explicit::FTCS::Step(
        ctx.U_0, 
        ctx.U_1, 
        Lower_DiffNumber);
        //Apply Boundary Conditions To New Field
        ctx.U_1.ApplyBoundaryCondition(ctx.boundaries.TopWall);
        ctx.U_1.ApplyBoundaryCondition(ctx.boundaries.BottomWall);
        if (i==n)
        {
            //U_nPlus1 = U_1
            ctx.U_n.Swap(ctx.U_1);
            break;
        }
        //update
        ctx.U_0.Swap(ctx.U_1);
    }
}


ctx.WriteInitialOutputs();

auto Coeffs = Discretization::Explicit::DuFortFrankel::CalculateCoefficients(
    ctx.params.DiffNumber.GetValue());

std::size_t TotalTimeLevel = ctx.params.NumTimeLevels.GetValue();

for(std::size_t TimeLevel_n = 1; TimeLevel_n <= TotalTimeLevel; TimeLevel_n++)
{

//This Condition Stops The Loop Solver Iterations by Pressing ESC
if (isEscPressed())
{
    std::cout << "\nESC pressed. Exiting program now..." << '\n';
    return 0;
}

Discretization::Explicit::DuFortFrankel::Step(
ctx.U_nMinus1,
ctx.U_n,
ctx.U_nPlus1,
Coeffs);

ctx.Common(TimeLevel_n);

if(ctx.GetBreakCondition())
{
    break;
}

}//End of Solver Loop


ctx.plot.LiveWindow.Close();

std::cout << "Done" << '\n';
std::cin.get();
return 0;
}//End of main
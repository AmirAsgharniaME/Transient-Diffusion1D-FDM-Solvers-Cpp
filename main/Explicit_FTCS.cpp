
// C++ Libraries
#include <iostream>

//Local Files
#include "Discretization/SolverScheme/SolverScheme.hpp"
#include "SimulationSetup/SimulationContext/SimulationContext.hpp"
#include "Utilities/KeyboardHandler/KeyboardHandler.hpp"
#include "Discretization/Explicit/FTCS/FTCS.hpp"
#include "Discretization/Explicit/FTCS/FTCS_Stability.hpp"



int main() 
{
//-------------------------------------------------------------------------------------------------
//Simulation Setup
SimulationContext ctx(SolverScheme::FTCS);
//-----------------------------------------------------------------------------------------------
//Stabilization and Time Step
Discretization::Explicit::FTCS::Stabilize(ctx.params);

ctx.StartUp(); //U_n = U_0
ctx.WriteInitialOutputs();
std::size_t TotalTimeLevel = ctx.params.NumTimeLevels.GetValue();

for(std::size_t TimeLevel_n = 1; TimeLevel_n <= TotalTimeLevel; TimeLevel_n++)
{

//This Condition Stops The Loop Solver Iterations by Pressing ESC
if (isEscPressed())
{
    std::cout << "\nESC pressed. Exiting program now..." << '\n';
    return 0;
}
Discretization::Explicit::FTCS::Step(
ctx.U_n,
ctx.U_nPlus1,
ctx.params.DiffNumber.GetValue());
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
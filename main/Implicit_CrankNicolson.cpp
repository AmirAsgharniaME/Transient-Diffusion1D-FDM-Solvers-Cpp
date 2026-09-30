

// C++ Libraries
#include <iostream>

//Local Files
#include "Discretization/Implicit/CrankNicolson/CrankNicolsonAssembler.hpp"
#include "Discretization/SolverScheme/SolverScheme.hpp"
#include "SimulationSetup/SimulationContext/SimulationContext.hpp"
#include "Utilities/KeyboardHandler/KeyboardHandler.hpp"
#include "Discretization/Implicit/Laasonen/LaasonenAssembler.hpp"
#include "LinearSolvers/Matrix/CoefficientMatrix.hpp"
#include "Discretization/Implicit/Boundary/EnforceBoundary.hpp"
#include "LinearSolvers/GaussianElimination/GaussianElimination.hpp"
#include "LinearSolvers/ThomasAlgorithm/ThomasAlgorithm.hpp"

int main() 
{
//-------------------------------------------------------------------------------------------------
//Simulation Setup
SimulationContext ctx(SolverScheme::CrankNicolson);
//-----------------------------------------------------------------------------------------------
//Setup
ctx.StartUp(); //U_n = U_0
//Create Coefficient Matrix For Gaussian Elimination
// CoefficientMatrix A(ctx.grid);
// Discretization::Implicit::CrankNicolson::AssembleMatrix(A,ctx.params.DiffNumber.GetValue());
// Discretization::Implicit::Boundary::Enforce(A,ctx.boundaries);

//-------------------------------------------------------------------------------------------------
// //Create Tridiagonal Matrix for Thomas Algorithm
TridiagonalMatrix A3(ctx.grid);
Discretization::Implicit::CrankNicolson::AssembleMatrix(A3,ctx.params.DiffNumber.GetValue());
Discretization::Implicit::Boundary::Enforce(A3,ctx.boundaries);

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
// //Gaussian Elimination Setup
// CoefficientMatrix A_Copy = A;
// RHS RHS1D(ctx.grid);
// Discretization::Implicit::CrankNicolson::AssembleRHS(RHS1D, ctx.U_n,ctx.params.DiffNumber.GetValue());
// Discretization::Implicit::Boundary::Enforce(RHS1D, ctx.U_n,ctx.boundaries);
// GaussianElimination::Solve_nPlus1(A_Copy,RHS1D,ctx.U_nPlus1);

// //Thomas Algorithm Setup
TridiagonalMatrix A3_Copy = A3;
RHS RHS1D(ctx.grid);
Discretization::Implicit::CrankNicolson::AssembleRHS(RHS1D, ctx.U_n,ctx.params.DiffNumber.GetValue());
Discretization::Implicit::Boundary::Enforce(RHS1D, ctx.U_n,ctx.boundaries);
ThomasAlgorithm::Solve_nPlus1(A3_Copy,RHS1D,ctx.U_nPlus1);

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
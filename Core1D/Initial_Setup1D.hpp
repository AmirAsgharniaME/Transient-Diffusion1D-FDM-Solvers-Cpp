#pragma once

#include "Core1D/Geometry1D/Geometry1D.hpp"
#include "Core1D/Mesh1D/Mesh1D.hpp"
#include "Core1D/Field1D/Field1D.hpp"
#include "Core1D/BoundaryPoint/BoundaryPoint.hpp"
#include "Utilities/Parameter/Parameter.hpp"
#include "Config/SolverInputs1D.hpp"
#include "Config/PathManager.hpp"
#include "Verification/AnalyticalDiffusion1D/AnalyticalDiffusion1D.hpp"
#include "Core1D/FileWriter1D/FileWriter1D.hpp"
#include "Config/FileNameCreator.hpp"
#
// ================================================
// 1-Intial Setup: 
// ================================================
// 1-Defining Geometry 
// 2-Defining Mesh
// 3-Defining Parameters(nu,dt,DiffNumber,Tolerance)
// 4-Defining Field
// 5-Apply Initial Conditions To Field_0
// 6-Defining physical boundaries
// 7-Apply Boundary Conditions To Field_0
// ================================================

struct Initial_Setup1D
{
//Defining Geometry
Geometry1D Length;
//-------------------------------------------------------------------------------------------------
//Defining Mesh
Mesh1D mesh1D;
//-------------------------------------------------------------------------------------------------
//Defining Parameters
    Parameter<double> nu;
    Parameter<std::size_t> NumTimeLevels;
    Parameter<double> dt;
    Parameter<double> Tolerance;
    Parameter<double> DiffNumber;
//-------------------------------------------------------------------------------------------------
//Defining Field
Field1D U_0;
Field1D U_n;
Field1D U_nPlus1;
//-------------------------------------------------------------------------------------------------
//Defining physical boundaries
BoundaryPoint TopWall;
BoundaryPoint BottomWall;
//-------------------------------------------------------------------------------------------------
//Defining Analytical Solution for Steady State
AnalyticalDiffusion1D U_Analytical_1D;


    Initial_Setup1D()
        :Length(SolverInputs::Geometry::Length, LineOrientation::Vertical),
         mesh1D(Length, SolverInputs::Mesh::N),
         U_0(Length, mesh1D),
         U_n(Length, mesh1D),
         U_nPlus1(Length, mesh1D),
         TopWall(BoundaryType::Dirichlet, BoundaryLocation::Top, SolverInputs::BoundaryCondition::TopWallValue),
         BottomWall(BoundaryType::Dirichlet, BoundaryLocation::Bottom, SolverInputs::BoundaryCondition::BottomWallValue),
         U_Analytical_1D(mesh1D, Length, TopWall, BottomWall)
    {
        nu.SetValue(SolverInputs::Physics::nu);
        NumTimeLevels.SetValue(SolverInputs::Solver::NumTimeLevels);
        dt.SetValue(SolverInputs::Solver::dt);
        Tolerance.SetValue(SolverInputs::Solver::Tolerance);
        DiffNumber.SetValue((nu.GetValue() * dt.GetValue()) / (mesh1D.Get_Delta() * mesh1D.Get_Delta()));
        //-------------------------------------------------------------------------------------------------
        //Apply Initial Conditions To Field
        U_0.SetIntitialProfile(SolverInputs::InitialCondition::Profile1D);
        //-------------------------------------------------------------------------------------------------
        //Apply Boundary Conditions To Field
        U_0.ApplyBoundaryCondition(BoundaryLocation::Top, TopWall);
        U_0.ApplyBoundaryCondition(BoundaryLocation::Bottom, BottomWall);
        //-------------------------------------------------------------------------------------------------
        //Write the Initial Field1D to a file
        FileWriter1D::WriteField1D
            (
            NameCreator::CreateName(FieldName::U,0.0),
            U_0,
            mesh1D,
            PathManager::GetOutputPath(OutputCategory::Initial)
            );
    //-------------------------------------------------------------------------------------------------
        //Write the Exact Analytical Solution to a file
        FileWriter1D::WriteField1D
            (
            NameCreator::CreateName(FieldName::UAnalytical,"Steady_State"),
            U_Analytical_1D,
            mesh1D,
            PathManager::GetOutputPath(OutputCategory::Analytical)
            );
        //-------------------------------------------------------------------------------------------------

    }
};

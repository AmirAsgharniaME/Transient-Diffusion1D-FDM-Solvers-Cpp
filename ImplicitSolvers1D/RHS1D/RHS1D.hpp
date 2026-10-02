#pragma once

#include <vector>
#include <cstddef>
#include "Core1D/Field1D/Field1D.hpp"
#include "Config/SolverScheme.hpp"

class RHS1D
{


private:
    SolverScheme Scheme;
    std::size_t N;
    std::vector<double> RHSValues;

    void Create_RHS_Laasonen(const Field1D& Field1D_Obj) noexcept;
    void Create_RHS_CrankNicolson( const Field1D& Field1D_Obj, double DiffNumber_) noexcept;
  


public:

    explicit RHS1D(
        SolverScheme Scheme_,
        const Field1D& Field1D_Obj,
        double DiffNumber_ );




    void SetValue(std::size_t PIndex, double Value_) noexcept
    {
        RHSValues[PIndex] = Value_;
    }

   [[nodiscard]]  double GetValue(std::size_t PIndex) const noexcept
    {
        return RHSValues[PIndex];
    }

   [[nodiscard]]  std::size_t GetSize() const noexcept
    {
         return RHSValues.size();
    }
  
    // Just For Reading
    [[nodiscard]] double operator[](std::size_t Index_) const noexcept
    {
        return RHSValues[Index_];
    }

    //For both Reading and Writing
    [[nodiscard]] double& operator[](std::size_t Index_) noexcept
    {
        return RHSValues[Index_];
    }

    void ApplyBoundaryCondition(const Field1D& Field1D_Obj) noexcept
    {
         //Apply Dirichlet boundary condition To RHS
        RHSValues[0] = Field1D_Obj[0];
        RHSValues[N - 1] = Field1D_Obj[N - 1];
    }

};

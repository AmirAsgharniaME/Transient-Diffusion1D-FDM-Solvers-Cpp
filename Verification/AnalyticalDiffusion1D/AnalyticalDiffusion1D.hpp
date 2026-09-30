#pragma once

#include <cstddef>
#include <vector>

#include "Core1D/Mesh1D/Mesh1D.hpp"
#include "Core1D/Geometry1D/Geometry1D.hpp"
#include "Core1D/BoundaryPoint/BoundaryPoint.hpp"
#include "Config/SolverInputs1D.hpp"

class AnalyticalDiffusion1D
{
public:
    explicit AnalyticalDiffusion1D(
        const Mesh1D& Mesh1D_Obj,
        const Geometry1D& Geometry1D_Obj,
        const BoundaryPoint& TopWall_Obj,
        const BoundaryPoint& BottomWall_Obj
    );

    explicit AnalyticalDiffusion1D(
        double Time_,
        const Mesh1D& Mesh1D_Obj,
        const Geometry1D& Geometry1D_Obj,
        const BoundaryPoint& TopWall_Obj,
        const BoundaryPoint& BottomWall_Obj
    );

    [[nodiscard]] std::size_t GetSize() const
    {
        return AnalyticalValues.size();
    }

    [[nodiscard]] const std::vector<double>& GetAnalyticalField1D() const noexcept
    {
        return AnalyticalValues;
    }

    [[nodiscard]] double operator[](std::size_t Index_) const noexcept
    {
        return AnalyticalValues[Index_];
    }

    [[nodiscard]] double& operator[](std::size_t Index_) noexcept
    {
        return AnalyticalValues[Index_];
    }

    [[nodiscard]] double GetValue(std::size_t Index) const
    {
        return AnalyticalValues[Index];
    }

private:
    const std::vector<double> InitialProfile =
        SolverInputs::InitialCondition::Profile1D;

    double nu =
        SolverInputs::Physics::nu;

    double Time = 0.0;


    std::size_t N = 0;

    std::vector<double> Grid1D;
    std::vector<double> AnalyticalValues;

    double Length = 0.0;

    double TopWallVlaue = 0.0;
    double BottomWallValue = 0.0;

    void Claculate_Steady_State_Analytical1D(
        const Mesh1D& Mesh1D_Obj
    );

    void Claculate_Transient_Analytical1D_n(
        double Time_,
        const Mesh1D& Mesh1D_Obj
    );
};

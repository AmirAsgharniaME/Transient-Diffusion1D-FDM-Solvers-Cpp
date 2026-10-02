#pragma once

#include "Core/Boundary/Boundary.hpp"

class SimulationBoundaries
{
public:
    Boundary TopWall;
    Boundary BottomWall;
    BoundaryType GetType() const noexcept
    {
        return Type;
    }

    explicit SimulationBoundaries();

private:
    BoundaryType Type;
};

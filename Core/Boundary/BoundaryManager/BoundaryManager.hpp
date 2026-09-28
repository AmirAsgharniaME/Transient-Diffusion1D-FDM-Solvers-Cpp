#pragma once
#include <cstdint>

enum class BoundaryLocation : std::uint8_t
{
    Top,
    Bottom,
    Right,
    Left
};

enum class BoundaryType : std::uint8_t
{
    Dirichlet, // Fixed value (e.g., fixed temperature)
    Neumann,   // Fixed flux / derivative (e.g., insulated wall: dT/dx = 0)
    Robin      // Mixed / convection condition
};

enum class Orientation : std::uint8_t
{
    Horizontal,
    Vertical
};
#pragma once

enum class BoundaryLocation
{
    Top,
    Bottom,
    Right,
    Left
};

// Boundary condition types in CFD / Heat Transfer
enum class BoundaryType
{
    Dirichlet, // Fixed value (e.g., fixed temperature)
    Neumann,   // Fixed flux / derivative (e.g., insulated wall: dT/dx = 0)
    Robin      // Mixed / convection condition
};

class BoundaryPoint
{
private:
    double BoundaryValue;
    BoundaryType Type;
    BoundaryLocation Location;

public:
    // Constructor directly inlined inside the class body
    constexpr BoundaryPoint(BoundaryType Type_, BoundaryLocation Location_, double BoundaryValue_) noexcept
        : BoundaryValue(BoundaryValue_),
          Type(Type_),
          Location(Location_)
    {}

    // Public Getters
    [[nodiscard]] double GetValue() const noexcept
    {
        return BoundaryValue;
    }

    [[nodiscard]] BoundaryType GetType() const noexcept
    {
        return Type;
    }

    [[nodiscard]] BoundaryLocation GetLocation() const noexcept
    {
        return Location;
    }

    // Public Setters
    void SetValue(double BoundaryValue_) noexcept
    {
        BoundaryValue = BoundaryValue_;
    }

    void SetType(BoundaryType Type_) noexcept
    {
        Type = Type_;
    }

    void SetLocation(BoundaryLocation Location_) noexcept
    {
        Location = Location_;
    }
};

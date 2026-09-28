#pragma once

#include "Core/Boundary/BoundaryManager/BoundaryManager.hpp"

class Boundary
{
private:
    double BoundaryValue;
    BoundaryType Type;
    Orientation orientation;
    BoundaryLocation Location;



public:
    // Constructor directly inlined inside the class body
    constexpr Boundary(
        BoundaryType Type_,
        Orientation Orientation_,
        BoundaryLocation Location_, 
        double BoundaryValue_) noexcept
        : BoundaryValue(BoundaryValue_),
          Type(Type_),
          orientation(Orientation_),
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
    [[nodiscard]] Orientation GetOrientation() const noexcept
    {
        return orientation;
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

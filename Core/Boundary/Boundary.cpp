#include "Core/Boundary/Boundary.hpp"

Boundary::Boundary(
    BoundaryType Type_,
    Orientation Orientation_,
    BoundaryLocation Location_,
    double BoundaryValue_) noexcept
    : BoundaryValue(BoundaryValue_),
      Type(Type_),
      orientation(Orientation_),
      Location(Location_)
{}

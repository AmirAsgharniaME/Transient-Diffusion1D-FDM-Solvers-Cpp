#include "Core/Geometry/Geometry.hpp"

Geometry::Geometry(double Length_)
    : Length(Length_)
{
    if (Length_ <= 0.0)
    {
        throw std::invalid_argument("Length must be greater than zero.");
    }
}

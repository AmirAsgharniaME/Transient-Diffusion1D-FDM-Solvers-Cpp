#pragma once

#include <stdexcept>

enum class LineOrientation
{
    Horizontal,
    Vertical
};

class Geometry1D
{
private:
    double Length;
    LineOrientation Line_Orientation;

public:
    explicit Geometry1D(double Length_, LineOrientation Orientation_)
        : Length(Length_),
          Line_Orientation(Orientation_)
    {
        if (Length_ <= 0.0)
        {
            throw std::invalid_argument("Length must be greater than zero.");
        }
    }

    [[nodiscard]] double GetLength() const noexcept
    {
        return Length;
    }

    [[nodiscard]] LineOrientation GetLineOrientation() const noexcept
    {
        return Line_Orientation;
    }

    void SetLength(double Length_)
    {
        if (Length_ <= 0.0)
        {
            throw std::invalid_argument("Length must be greater than zero.");
        }
        Length = Length_;
    }

    void SetLineOrientation(LineOrientation Orientation_) noexcept
    {
        Line_Orientation = Orientation_;
    }
};

#pragma once

#include <stdexcept>

class Geometry
{
private:
    double Length;

public:
    explicit Geometry(double Length_);

    [[nodiscard]] double GetLength() const noexcept
    {
        return Length;
    }


    void SetLength(double Length_)
    {
        if (Length_ <= 0.0)
        {
            throw std::invalid_argument("Length must be greater than zero.");
        }
        Length = Length_;
    }
};

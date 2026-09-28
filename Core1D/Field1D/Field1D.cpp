#include "Core1D/Field1D/Field1D.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>


Field1D::Field1D(const Geometry1D& Geometry1D_Obj,const Mesh1D& Mesh1D_Obj)
    : N(Mesh1D_Obj.Get_N()),
      Field1D_Values(N, 0.0),
      Line_Orientation(Geometry1D_Obj.GetLineOrientation())
{
}


void Field1D::SetAllValues(double Value_) noexcept
{
    std::fill(Field1D_Values.begin(), Field1D_Values.end(), Value_);
}

void  Field1D::SetIntitialProfile(const std::vector<double> InitialProfile_)
{
    if (InitialProfile_.size() != N)
    {
        throw std::invalid_argument("InitialProfile_ must have the same size as the mesh.");
    }
    for (std::size_t i = 0; i < N; ++i)
    {
        Field1D_Values[i] = InitialProfile_[i];
    }

}


void Field1D::Print() const noexcept
{
    std::cout << std::scientific << std::setprecision(3);

    std::cout << "\n"
              << std::left << std::setw(10) << "Index"
              << std::right << std::setw(20) << "Field Value"
              << '\n';

    std::cout << std::string(30, '-') << '\n';

    for (std::size_t i = 0; i < N; ++i)
    {
        std::cout << std::left << std::setw(10) << i
                  << std::right << std::setw(20) << Field1D_Values[i]
                  << '\n';
    }
}

#include "Core/Field/Field.hpp"

#include <iomanip>
#include <iostream>
#include <string>


Field::Field (const Mesh& Mesh_Obj)
    : N(Mesh_Obj.Get_N()),
      field(N, 0.0)
{
}


void Field::SetAllValues(double Value_) noexcept
{
    for (std::size_t i = 0; i < N; ++i)
    {
        field[i] = Value_;
    }   
}

void  Field::SetIntitialProfile(const std::vector<double> InitialProfile_)
{
    if (InitialProfile_.size() != N)
    {
        throw std::invalid_argument("InitialProfile_ must have the same size as the mesh.");
    }
    for (std::size_t i = 0; i < N; ++i)
    {
        field[i] = InitialProfile_[i];
    }

}


void Field::Print() const noexcept
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
                  << std::right << std::setw(20) << field[i]
                  << '\n';
    }
}


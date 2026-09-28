#include "Core1D/Mesh1D/Mesh1D.hpp"

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <iomanip>



Mesh1D::Mesh1D(const Geometry1D& Geometry1D_Obj,std::size_t Nodes_)
    :N(Nodes_),
     Length(Geometry1D_Obj.GetLength()),
     Delta(Length / static_cast<double>(N-1)),
     Grid1D(N,0.0)
{
    if (N < 2)
    {
        throw std::invalid_argument("The number of mesh nodes must be at least 2.");
    }
    MakeGrid();
}

    // public Setters
    void Mesh1D::Set_N(std::size_t N_)
    {
        if (N_ < 2)
        {
            throw std::invalid_argument("The number of mesh nodes must be at least 2.");
        }
        N = N_;
        Delta = Length / static_cast<double>(N-1);
        Grid1D.resize(N);
        MakeGrid();
    }


    // public Methods
    void Mesh1D::Print() const noexcept
    {
        std::cout << std::scientific << std::setprecision(3);

        std::cout << "\n"
                    << std::left << std::setw(10) << "Index"
                    << std::right << std::setw(20) << "Position"
                    << '\n';

        std::cout << std::string(30, '-') << '\n';

            for (std::size_t i = 0; i < N; ++i) 
            {
                std::cout << std::left << std::setw(10) << i
                            << std::right << std::setw(20) <<Grid1D[i]
                            << '\n';
            }

    }


    void Mesh1D::MakeGrid()
    {
        for (std::size_t Index = 0; Index < N; ++Index)
        {
            Grid1D[Index] = static_cast<double>(Index) * Delta;
        }
        Grid1D[N - 1] = Length; // To guarantee that it will take the exact value of Length.
    }



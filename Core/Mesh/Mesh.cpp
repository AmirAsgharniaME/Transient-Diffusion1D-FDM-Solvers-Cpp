#include "Core/Mesh/Mesh.hpp"

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <iomanip>



Mesh::Mesh(const Geometry& Geometry_Obj,std::size_t Nodes_)
    :N(Nodes_),
     Length(Geometry_Obj.GetLength()),
     Delta(Length / static_cast<double>(N-1)),
     Grid(N,0.0)
{
    if (N < 2)
    {
        throw std::invalid_argument("The number of mesh nodes must be at least 2.");
    }
    MakeGrid();
}

    // public Setters
    void Mesh::Set_N(std::size_t N_)
    {
        if (N_ < 2)
        {
            throw std::invalid_argument("The number of mesh nodes must be at least 2.");
        }
        N = N_;
        Delta = Length / static_cast<double>(N-1);
        Grid.resize(N);
        MakeGrid();
    }


    // public Methods
    void Mesh::Print() const noexcept
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
                            << std::right << std::setw(20) <<Grid[i]
                            << '\n';
            }

    }


    void Mesh::MakeGrid()
    {
        for (std::size_t Index = 0; Index < N; ++Index)
        {
            Grid[Index] = static_cast<double>(Index) * Delta;
        }
        Grid[N - 1] = Length; // To guarantee that it will take the exact value of Length.
    }



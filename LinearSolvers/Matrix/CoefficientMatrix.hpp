#pragma once
#include <vector>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include "Core/Mesh/Mesh.hpp"

class CoefficientMatrix
{


private:

    std::size_t N;

    // Flat 1D vector of size N*N for A[j*ncols + i] in 1D : N = nrows = ncols
    std::vector<double> A; 

public:
    CoefficientMatrix(const Mesh& Mesh_Obj)
        :N(Mesh_Obj.Get_N()),
         A(N * N, 0.0)
    {}

    [[nodiscard]] std::size_t GetRows() const noexcept
    {
        return N;
    }

    [[nodiscard]] std::size_t GetCols() const noexcept
    {
        return N;
    }

    [[nodiscard]] double GetValue(std::size_t j, std::size_t i) const noexcept
    {
        return A[j * N + i];
    }

    void SetValue(std::size_t j, std::size_t i, double Value_) noexcept
    {
        A[j * N + i] = Value_;
    }

    // Returns a pointer to the start of the row 'j' for row-major access (A[j][i]).
    // This enables 2D array syntax on a flat 1D vector with zero runtime overhead.
    [[nodiscard]] double* operator[](std::size_t j) noexcept
    {
        return &A[j * N];
    }

    // Returns a const pointer to the start of the row 'j' for read-only row-major access.
    [[nodiscard]] const double* operator[](std::size_t j) const noexcept
    {
        return &A[j * N];
    }

    void Print() const noexcept
{
    std::cout << std::fixed << std::setprecision(4);

    for (std::size_t j = 0; j < N; ++j)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            std::cout << std::setw(15) << A[j*N + i];
        }

        std::cout << '\n';
    }
}


};

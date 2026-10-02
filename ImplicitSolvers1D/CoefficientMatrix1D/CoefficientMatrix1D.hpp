#pragma once
#include <vector>
#include <cstddef>
#include "Core1D/Mesh1D/Mesh1D.hpp"
#include "Core1D/Field1D/Field1D.hpp"
#include "Config/SolverScheme.hpp"

class CoefficientMatrix1D
{


private:
    SolverScheme Scheme;
    std::size_t N;
    std::vector<double> AValues; // Flat 1D vector of size N*N for A[j*ncols + i] in 1D N = nrows = ncols
    double r;

    void Create_A_Laasonen() noexcept;
    void Create_A_CrankNicolson() noexcept;

public:
    CoefficientMatrix1D(
        SolverScheme Scheme_,
        const Mesh1D& Mesh1D_Obj,
        double DiffNumber_
    );

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
        return AValues[j * N + i];
    }

    void SetValue(std::size_t j, std::size_t i, double Value_) noexcept
    {
        AValues[j * N + i] = Value_;
    }

    // Returns a pointer to the start of the row 'j' for row-major access (A[j][i]).
    // This enables 2D array syntax on a flat 1D vector with zero runtime overhead.
    [[nodiscard]] double* operator[](std::size_t j) noexcept
    {
        return &AValues[j * N];
    }

    // Returns a const pointer to the start of the row 'j' for read-only row-major access.
    [[nodiscard]] const double* operator[](std::size_t j) const noexcept
    {
        return &AValues[j * N];
    }

    void Print() const noexcept;

    void SetBoundaryConditions() noexcept
    {
        AValues[0 * N + 0] = 1.0;
        AValues[(N - 1) * N + (N - 1)] = 1.0;
    }


};

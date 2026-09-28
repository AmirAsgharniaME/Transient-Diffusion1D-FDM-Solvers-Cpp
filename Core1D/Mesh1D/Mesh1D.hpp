#pragma once

#include <cstddef>
#include <vector>
#include "Core1D/Geometry1D/Geometry1D.hpp"

class Mesh1D
{
private:
    std::size_t N; // Number of nodes; intervals = N - 1
    double Length;
    double Delta;  // Grid spacing: Delta = Length / (N - 1)
    std::vector<double> Grid1D;

private:
    void MakeGrid();

public:
    // Constructor
    explicit Mesh1D(const Geometry1D& Geometry1D_Obj, std::size_t Nodes_);

public:
    // Public Getters
    [[nodiscard]] std::size_t Get_N() const noexcept { return N; }
    [[nodiscard]] double Get_Delta() const noexcept { return Delta; }

    [[nodiscard]] double Get_GridValue(std::size_t Index) const noexcept
    {
        return Grid1D[Index];
    }

    [[nodiscard]] const std::vector<double>& Get_Grid() const noexcept
    {
        return Grid1D;
    }

    // Direct read-only indexing operator for simple syntax: mesh[i]
    [[nodiscard]] double operator[](std::size_t Index) const noexcept
    {
        return Grid1D[Index];
    }

public:
    // Public Setters
    void Set_N(std::size_t N_);

public:
    // Public Methods
    void Print() const noexcept;
};

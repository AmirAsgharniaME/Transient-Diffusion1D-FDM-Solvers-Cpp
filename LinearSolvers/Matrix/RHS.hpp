#pragma once

#include <vector>
#include <cstddef>
#include "Core/Mesh/Mesh.hpp"


class RHS
{


private:
    std::size_t N;
    std::vector<double> rhs;

public:

    explicit RHS(const Mesh& Mesh_Obj)
        :N(Mesh_Obj.Get_N()),
         rhs(N, 0.0)
    {}

    void SetValue(std::size_t PIndex, double Value_) noexcept
    {
        rhs[PIndex] = Value_;
    }

   [[nodiscard]]  double GetValue(std::size_t PIndex) const noexcept
    {
        return rhs[PIndex];
    }

   [[nodiscard]]  std::size_t GetSize() const noexcept
    {
         return rhs.size();
    }
  
    // Just For Reading
    [[nodiscard]] double operator[](std::size_t Index_) const noexcept
    {
        return rhs[Index_];
    }

    //For both Reading and Writing
    [[nodiscard]] double& operator[](std::size_t Index_) noexcept
    {
        return rhs[Index_];
    }

};

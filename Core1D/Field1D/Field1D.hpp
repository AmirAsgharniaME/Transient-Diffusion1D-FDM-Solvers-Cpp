#pragma once

#include <cstddef>
#include <vector>
#include <utility>
#include "Core1D/Geometry1D/Geometry1D.hpp"
#include "Core1D/Mesh1D/Mesh1D.hpp"
#include "Core1D/BoundaryPoint/BoundaryPoint.hpp"


class Field1D
{
private:
    std::size_t N;
    std::vector<double> Field1D_Values;
    LineOrientation Line_Orientation;

public:
    // Constructor
    explicit Field1D(const Geometry1D& Geometry1D_Obj,const Mesh1D& Mesh1D_Obj);

    // Getters
    [[nodiscard]] std::size_t GetSize() const noexcept
    {
        return N;
    }

    [[nodiscard]] double GetValue(std::size_t Index_) const noexcept
    {
        return Field1D_Values[Index_];
    }

    [[nodiscard]] const std::vector<double>& GetField1D() const noexcept
    {
        return Field1D_Values;
    }
    
    // Just For Reading
    [[nodiscard]] double operator[](std::size_t Index_) const noexcept
    {
        return Field1D_Values[Index_];
    }
    //For both Reading and Writing
    [[nodiscard]] double& operator[](std::size_t Index_) noexcept
    {
        return Field1D_Values[Index_];
    }

    // Setters
    void SetValue(std::size_t Index_, double Value_) noexcept
    {
        Field1D_Values[Index_] = Value_;
    }

    void SetAllValues(double Value_) noexcept;

    void SetIntitialProfile(const std::vector<double> InitialProfile_);


    void ApplyBoundaryCondition(BoundaryLocation Location_,const BoundaryPoint& Boundary_Obj) noexcept
    {
        if (N == 0)
        {
            return;
        }
        if (Line_Orientation == LineOrientation::Horizontal)
        {
            if (Location_ == BoundaryLocation::Left)
            {
                Field1D_Values[0] = Boundary_Obj.GetValue();
            }
            if (Location_ == BoundaryLocation::Right)
            {
                Field1D_Values[N - 1] = Boundary_Obj.GetValue();
            }

        }
        else if (Line_Orientation== LineOrientation::Vertical)
        {
            if (Location_ == BoundaryLocation::Bottom)
            {
                 Field1D_Values[0] = Boundary_Obj.GetValue();
            }
            if (Location_ == BoundaryLocation::Top)
            {
                Field1D_Values[N - 1] = Boundary_Obj.GetValue();
            }

        }

    }   

    

    // Methods
    // Inlined in header: Called frequently in the time-marching loop (T_old <-> T_new)
    void Swap(Field1D& Other) noexcept
    {
        using std::swap;
        swap(N, Other.N);
        Field1D_Values.swap(Other.Field1D_Values); // O(1) pointer swap inside std::vector
    }

    void Print() const noexcept;
};

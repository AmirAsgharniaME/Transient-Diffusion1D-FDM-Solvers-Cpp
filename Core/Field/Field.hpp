#pragma once

#include <cstddef>
#include <vector>
#include <utility>
#include "Core/Mesh/Mesh.hpp"
#include "Core/Boundary/Boundary.hpp"

class Field
{

private:
    std::size_t N;
    std::vector<double> field;

public:
    // Constructor
    explicit Field(const Mesh& Mesh_Obj);

    // Virtual destructor: essential for base classes in inheritance hierarchies
    virtual ~Field() = default;

    // Getters
    [[nodiscard]] std::size_t GetSize() const noexcept
    {
        return N;
    }

    [[nodiscard]] double GetValue(std::size_t Index_) const noexcept
    {
        return field[Index_];
    }

    [[nodiscard]] const std::vector<double>& GetField() const noexcept
    {
        return field;
    }
    
    // Just For Reading
    [[nodiscard]] double operator[](std::size_t Index_) const noexcept
    {
        return field[Index_];
    }
    //For both Reading and Writing
    [[nodiscard]] double& operator[](std::size_t Index_) noexcept
    {
        return field[Index_];
    }

    // Setters
    void SetValue(std::size_t Index_, double Value_) noexcept
    {
        field[Index_] = Value_;
    }

    void SetAllValues(double Value_) noexcept;

    void SetIntitialProfile(const std::vector<double> InitialProfile_);

    /*
    if the line orientation is horizontal, the boundary condition is applied at the left and right boundaries
    if the line orientation is vertical, the boundary condition is applied at the bottom and top boundaries
    */
    void ApplyBoundaryCondition(const Boundary& Boundary_Obj) noexcept
    {

        if (Boundary_Obj.GetOrientation() == Orientation::Horizontal)
        {
            if (Boundary_Obj.GetLocation() == BoundaryLocation::Left)
            {
                field[0] = Boundary_Obj.GetValue();
            }
            if (Boundary_Obj.GetLocation() == BoundaryLocation::Right)
            {
                field[N - 1] = Boundary_Obj.GetValue();
            }

        }
        else if (Boundary_Obj.GetOrientation()  == Orientation::Vertical)
        {
            if (Boundary_Obj.GetLocation() == BoundaryLocation::Bottom)
            {
                 field[0] = Boundary_Obj.GetValue();
            }
            if (Boundary_Obj.GetLocation() == BoundaryLocation::Top)
            {
                field[N - 1] = Boundary_Obj.GetValue();
            }

        }

    } 

    

    // Methods
    // Inlined in header: Called frequently in the time-marching loop (T_old <-> T_new)
    void Swap(Field& Other) noexcept
    {
        using std::swap;
        swap(N, Other.N);
        field.swap(Other.field); // O(1) pointer swap inside std::vector
    }

    void Print() const noexcept;


};

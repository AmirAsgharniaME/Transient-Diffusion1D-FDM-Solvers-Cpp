#include "ExplicitSolvers1D/ExplicitSolvers1D.hpp"
#include <cstddef>

namespace  FTCS
{
 void Solve_nPlus1(
  const Field1D& Field1D_n_Obj,
  Field1D& Field1D_nPlus1_Obj,
  double DiffNumber) noexcept
 {

      for (size_t i = 1; i < Field1D_n_Obj.GetSize() - 1 ; i++)
      {
        double RHS = Field1D_n_Obj[i]
        + DiffNumber * (Field1D_n_Obj[i+1] - 2.0 * Field1D_n_Obj[i]
        + Field1D_n_Obj[i-1]);
        Field1D_nPlus1_Obj[i] = RHS;
      }

}

}

namespace  DUFORT_FRANKEL
{

void Solve_nPlus1(
  const Field1D& Field1D_n_Obj,
  const Field1D& Field1D_nminus1_Obj,
  Field1D& Field1D_nplus1_Obj,
 std::array<double, 2> CoeffsVector_) noexcept
  {

      for (size_t i = 1; i < Field1D_n_Obj.GetSize() - 1 ; i++)
      {
        double RHS = CoeffsVector_[0] * Field1D_nminus1_Obj[i] 
        + CoeffsVector_[1] * (Field1D_n_Obj[i+1] + Field1D_n_Obj[i-1]);
        Field1D_nplus1_Obj[i] = RHS;
      }

  }


}

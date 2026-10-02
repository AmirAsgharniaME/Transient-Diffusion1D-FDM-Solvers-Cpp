#include "LinearSolvers/Matrix/RHS/RHS.hpp"

RHS::RHS(const Mesh& Mesh_Obj)
    : N(Mesh_Obj.Get_N()),
      rhs(N, 0.0)
{}

#include "LinearSolvers/Matrix/CoefficientMatrix/CoefficientMatrix.hpp"

CoefficientMatrix::CoefficientMatrix(const Mesh& Mesh_Obj)
    : N(Mesh_Obj.Get_N()),
      A(N * N, 0.0)
{}

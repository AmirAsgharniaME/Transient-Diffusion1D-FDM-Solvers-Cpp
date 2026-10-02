#include "LinearSolvers/Matrix/TridiagonalMatrix/TridiagonalMatrix.hpp"

TridiagonalMatrix::TridiagonalMatrix(const Mesh& Mesh_Obj)
    : N(Mesh_Obj.Get_N()),
      LowerDiagonal(N, 0.0),
      MainDiagonal(N, 0.0),
      UpperDiagonal(N, 0.0)
{}

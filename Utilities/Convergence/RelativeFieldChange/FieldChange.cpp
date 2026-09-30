#include "Utilities/Convergence/RelativeFieldChange/FieldChange.hpp"
#include <cstddef>
#include <stdexcept>
#include <cmath>

namespace RelativeFieldChange
{
/*
Normalized L2 norm of the discrete time derivative between two consecutive time steps.

ReturnFieldChange computes:

    (t_scale / dt) * [ sqrt( sum_i (Field_nPlus1[i] - Field_n[i])^2 )
                       ---------------------------------------------------
                           sqrt( sum_i Field_nPlus1[i]^2 ) + epsilon   ]

where:
    Field_n       is the field at the current time step,
    Field_nPlus1  is the field at the next time step,
    dt              is the physical time step size (must be > 0),
    t_scale         is the characteristic physical time scale (e.g. L^2 / alpha),
                    used to make the stopping criterion non-dimensional and
                    independent of dt,
    epsilon         prevents division by zero.

In explicit schemes (FTCS, DuFort-Frankel), dividing by dt recovers the discrete
time derivative dU/dt, which directly reflects the steady-state spatial residual.
*/
double Return(const Field& Field_n_Obj,
                         const Field& Field_nPlus1_Obj,
                         const double dt,
                         const double t_scale)
{
    if (dt <= 0.0)
    {
        throw std::invalid_argument("Time step dt must be strictly positive.");
    }

    const std::size_t N = Field_n_Obj.GetSize();

    if (Field_nPlus1_Obj.GetSize() != N)
    {
        throw std::invalid_argument("Field sizes must match.");
    }

    double Sum1 = 0.0; // Σ (Field_nPlus1_obj[i] - Field_n_obj[i])^2
    double Sum2 = 0.0; // Σ (Field_nPlus1_obj[i])^2

    for (std::size_t i = 0; i < N; ++i)
    {
        const double Difference = Field_nPlus1_Obj[i] - Field_n_Obj[i];
        const double Field_nplus1 = Field_nPlus1_Obj[i];

        Sum1 += Difference * Difference;
        Sum2 += Field_nplus1 * Field_nplus1;
    }

    const double relative_change = std::sqrt(Sum1) / (std::sqrt(Sum2) + 1.0e-30);

    return (t_scale / dt) * relative_change;
}

}

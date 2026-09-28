#pragma once

#include "Core1D/Field1D/Field1D.hpp"

namespace RelativeFieldChange1D
{
    [[nodiscard]]double ReturnFieldChange(
        const Field1D& Field1D_n_Obj,
        const Field1D& Field1D_nPlus1_Obj,
        const double dt,
        const double t_scale
    );
}
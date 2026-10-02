#include "ImplicitSolvers1D/RHS1D/RHS1D.hpp"


RHS1D::RHS1D(
        SolverScheme Scheme_,
        const Field1D& Field1D_Obj,
        double DiffNumber_)
        :Scheme(Scheme_),
        N(Field1D_Obj.GetSize()),
        RHSValues(N, 0.0)
    
{
    if (Scheme == SolverScheme::Laasonen)
    {
        //Iterate through the interior nodes
        Create_RHS_Laasonen(Field1D_Obj);
    }
    else if (Scheme == SolverScheme::CrankNicolson)
    {
        //Iterate through the interior nodes
        Create_RHS_CrankNicolson(Field1D_Obj,DiffNumber_);
    }

}

void RHS1D::Create_RHS_Laasonen(const Field1D& Field1D_Obj) noexcept
{
    // ==============
    // Lassonen Method
    // ===============
    // EQ : -r * U[i - 1][n + 1] + (1.0 + 2.0 * r) * U[i][n + 1] - r * U[i + 1][n + 1] = RHS(U_n)
    // RHS = U[i][n]
    // Create RHS for Laasonen Method
    // ========================================
   

     // Iterate through the interior nodes
    for (std::size_t i = 1; i < N-1; i++)
    {
            RHSValues[i] = Field1D_Obj[i];
    }

}

void RHS1D::Create_RHS_CrankNicolson(const Field1D& Field1D_Obj, double DiffNumber_) noexcept
{
    // =====================================================================================
    // CrankNicolson Method
    // =====================================================================================
    //  EQ : -(r / 2) * U[i - 1]^(n + 1) + (1 + r) * U[i]^(n + 1) - (r / 2) * U[i + 1]^(n + 1)
    //  = (r / 2) * U[i - 1]^n + (1 - r) * U[i]^n + (r / 2) * U[i + 1]^n
    //  Create RHS for CrankNicolson Method  
    // =====================================================================================
    // Iterate through the interior nodes, excluding boundary nodes.
    double r_half = DiffNumber_ / 2.0;
    for (size_t i = 1; i < N-1; i++)
    {

        // Reading strictly from U_old_local preserves the n-th time level data.
        double rhs =(r_half) *   Field1D_Obj[i - 1]
                    + (1.0 - DiffNumber_) * Field1D_Obj[i]
                    + (r_half) * Field1D_Obj[i + 1];

        // Update the RHS vector with the new calculated implicit source term.
        RHSValues[i]=rhs;
    }

}




#pragma once
#include <iostream>
#include "Utilities/Parameter/Parameter.hpp"
#include "Core1D/Mesh1D/Mesh1D.hpp"

struct StabilityParams 
{
    Parameter<double>* dt_Obj;
    Parameter<double>* DiffNumber_Obj;
    const Parameter<double>* nu_Obj;
};

   inline void Stabilization(const Mesh1D& Mesh1D_obj,StabilityParams& Params)
{


// Stable_dt_Obj and Stable_DiffNumber are initialized with my inputs
    double Lower_dt = Params.dt_Obj->GetValue();
    double Lower_DiffNumber = Params.DiffNumber_Obj->GetValue();
    double delta = Mesh1D_obj.Get_Delta();
    double alpha = Params.nu_Obj->GetValue();

  std::cout << "Diffusion Number limit is 0.5 but Current Diffusion Number = "<< Params.DiffNumber_Obj->GetValue() << '\n';
  std::cout << "FTCS is Unstable with dt = " <<Params.dt_Obj->GetValue()<< '\n';

        double Reduction_Factor = 0.95;
        double Inv_delta = 1.0 / delta;

        while (Lower_DiffNumber >= 0.5)
        {
        Lower_dt= Reduction_Factor * Lower_dt;
        Lower_DiffNumber = alpha * Lower_dt *(Inv_delta * Inv_delta);
        }

        std::cout << "dt is has been reduced for the stability of FTCS method." << '\n';

        Params.dt_Obj->SetValue(Lower_dt);
        Params.DiffNumber_Obj->SetValue(Lower_DiffNumber);

        std::cout <<" New dt = "<<Lower_dt<<'\n';
        std::cout <<" New Diffusin Number = "<<Lower_DiffNumber<<'\n';

  }
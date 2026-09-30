#include "Verification/AnalyticalDiffusion1D/AnalyticalDiffusion1D.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>

AnalyticalDiffusion1D::AnalyticalDiffusion1D(
    const Mesh1D& Mesh1D_Obj,
    const Geometry1D& Geometry1D_Obj,
    const BoundaryPoint& TopWall_Obj,
    const BoundaryPoint& BottomWall_Obj
)
    : Time(0.0),
      N(Mesh1D_Obj.Get_N()),
      AnalyticalValues(N, 0.0),
      Length(Geometry1D_Obj.GetLength()),
      TopWallVlaue(TopWall_Obj.GetValue()),
      BottomWallValue(BottomWall_Obj.GetValue())
{
    Claculate_Steady_State_Analytical1D(Mesh1D_Obj);
}


AnalyticalDiffusion1D::AnalyticalDiffusion1D(
    double Time_,
    const Mesh1D& Mesh1D_Obj,
    const Geometry1D& Geometry1D_Obj,
    const BoundaryPoint& TopWall_Obj,
    const BoundaryPoint& BottomWall_Obj
)
    : Time(Time_),
      N(Mesh1D_Obj.Get_N()),
      AnalyticalValues(N, 0.0),
      Length(Geometry1D_Obj.GetLength()),
      TopWallVlaue(TopWall_Obj.GetValue()),
      BottomWallValue(BottomWall_Obj.GetValue())
{
    Claculate_Transient_Analytical1D_n(Time_, Mesh1D_Obj);
}


void AnalyticalDiffusion1D::Claculate_Steady_State_Analytical1D(
    const Mesh1D& Mesh1D_Obj
)
{
    if (Length <= 0.0)
    {
        throw std::invalid_argument(
            "AnalyticalDiffusion1D: Length must be greater than zero."
        );
    }

    const double Slope =
        (TopWallVlaue - BottomWallValue) / Length;

    for (std::size_t i = 0; i < N; ++i)
    {
        AnalyticalValues[i] =
            BottomWallValue + Slope * Mesh1D_Obj[i];
    }
}


void AnalyticalDiffusion1D::Claculate_Transient_Analytical1D_n(
    double Time_,
    const Mesh1D& Mesh1D_Obj
)
{
    if (Length <= 0.0)
    {
        throw std::invalid_argument(
            "AnalyticalDiffusion1D: Length must be greater than zero."
        );
    }

    if (Time_ < 0.0)
    {
        throw std::invalid_argument(
            "AnalyticalDiffusion1D: Time must not be negative."
        );
    }

    if (nu < 0.0)
    {
        throw std::invalid_argument(
            "AnalyticalDiffusion1D: Diffusion coefficient nu must not be negative."
        );
    }

    if (InitialProfile.size() != N)
    {
        throw std::invalid_argument(
            "AnalyticalDiffusion1D: InitialProfile size must be equal to mesh size."
        );
    }

    if (N < 2)
    {
        throw std::invalid_argument(
            "AnalyticalDiffusion1D: At least two mesh points are required."
        );
    }

    const double Slope =
        (TopWallVlaue - BottomWallValue) / Length;

    const std::size_t NumberOfModes = N - 1;

    std::vector<double> FourierCoefficients(NumberOfModes, 0.0);

    /*
     * محاسبه ضرایب سری فوریه:
     *
     * A_n = 2/L * integral(
     *     [f(x) - u_s(x)] * sin(n*pi*x/L)
     * ) dx
     *
     * انتگرال با روش ذوزنقه‌ای محاسبه می‌شود.
     */
    for (std::size_t Mode = 1; Mode <= NumberOfModes; ++Mode)
    {
        double Integral = 0.0;

        for (std::size_t i = 0; i < N - 1; ++i)
        {
            const double X_Left = Mesh1D_Obj[i];
            const double X_Right = Mesh1D_Obj[i + 1];

            const double Steady_Left =
                BottomWallValue + Slope * X_Left;

            const double Steady_Right =
                BottomWallValue + Slope * X_Right;

            const double Residual_Left =
                InitialProfile[i] - Steady_Left;

            const double Residual_Right =
                InitialProfile[i + 1] - Steady_Right;

            const double Sine_Left =
                std::sin(
                    static_cast<double>(Mode)
                    * M_PI
                    * X_Left
                    / Length
                );

            const double Sine_Right =
                std::sin(
                    static_cast<double>(Mode)
                    * M_PI
                    * X_Right
                    / Length
                );

            const double Integrand_Left =
                Residual_Left * Sine_Left;

            const double Integrand_Right =
                Residual_Right * Sine_Right;

            Integral +=
                0.5
                * (X_Right - X_Left)
                * (Integrand_Left + Integrand_Right);
        }

        FourierCoefficients[Mode - 1] =
            (2.0 / Length) * Integral;
    }

    for (std::size_t i = 0; i < N; ++i)
    {
        const double X = Mesh1D_Obj[i];

        const double SteadyValue =
            BottomWallValue + Slope * X;

        double TransientValue = 0.0;

        for (std::size_t Mode = 1; Mode <= NumberOfModes; ++Mode)
        {
            const double WaveNumber =
                static_cast<double>(Mode) * M_PI / Length;

            const double TemporalDecay =
                std::exp(
                    -nu
                    * WaveNumber
                    * WaveNumber
                    * Time_
                );

            const double SpatialMode =
                std::sin(WaveNumber * X);

            TransientValue +=
                FourierCoefficients[Mode - 1]
                * SpatialMode
                * TemporalDecay;
        }

        AnalyticalValues[i] =
            SteadyValue + TransientValue;
    }

    /*
     * اعمال مستقیم شرایط مرزی دیریکله.
     * این کار خطای عددی احتمالی سری فوریه در دو انتهای دامنه را حذف می‌کند.
     */
    AnalyticalValues.front() = BottomWallValue;
    AnalyticalValues.back() = TopWallVlaue;
}

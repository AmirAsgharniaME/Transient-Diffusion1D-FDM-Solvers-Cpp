#include "Verification/AnalyticalSolution/AnalyticalSolution.hpp"

#include <cmath>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>

namespace DiffusionEQ::AnalyticalSolution
{

namespace
{

// Accumulate Fourier sums with reduced cancellation error.
class CompensatedSum
{
public:
    void Add(long double Value)
    {
        const long double Adjusted = Value - Correction;
        const long double Next = Sum + Adjusted;
        Correction = (Next - Sum) - Adjusted;
        Sum = Next;
    }

    long double Get() const { return Sum; }

private:
    long double Sum = 0.0L;
    long double Correction = 0.0L;
};

struct Problem
{
    std::size_t N;
    long double Length;
    long double Bottom;
    long double Top;

    long double SteadyValue(std::size_t Index) const
    {
        const long double Fraction =
            static_cast<long double>(Index) / static_cast<long double>(N - 1);
        return (1.0L - Fraction) * Bottom + Fraction * Top;
    }
};

Problem Validate(
    const Field& Output,
    const Geometry& Geometry_Obj,
    const Mesh& Mesh_Obj,
    const SimulationBoundaries& Boundaries_Obj
)
{
    const std::size_t N = Mesh_Obj.Get_N();
    const double Length = Geometry_Obj.GetLength();
    const double Bottom = Boundaries_Obj.BottomWall.GetValue();
    const double Top = Boundaries_Obj.TopWall.GetValue();

    if (N < 2 || Output.GetSize() != N)
    {
        throw std::invalid_argument(
            "AnalyticalSolution: Output size must match a mesh with at least two nodes."
        );
    }
    if (!std::isfinite(Length) || Length <= 0.0)
    {
        throw std::invalid_argument(
            "AnalyticalSolution: Length must be positive and finite."
        );
    }
    if (Boundaries_Obj.BottomWall.GetType() != BoundaryType::Dirichlet
        || Boundaries_Obj.TopWall.GetType() != BoundaryType::Dirichlet
        || !std::isfinite(Bottom) || !std::isfinite(Top))
    {
        throw std::invalid_argument(
            "AnalyticalSolution: Finite Dirichlet boundary values are required."
        );
    }

    // The sine transform assumes a uniform mesh spanning [0, Length].
    const long double Tolerance =
        64.0L * std::numeric_limits<double>::epsilon()
        * static_cast<long double>(Length);
    for (std::size_t i = 0; i < N; ++i)
    {
        const long double Expected =
            static_cast<long double>(Length)
            * (static_cast<long double>(i) / static_cast<long double>(N - 1));
        if (!std::isfinite(Mesh_Obj[i])
            || std::abs(static_cast<long double>(Mesh_Obj[i]) - Expected) > Tolerance)
        {
            throw std::invalid_argument(
                "AnalyticalSolution: Mesh must be uniform and span [0, Length]."
            );
        }
    }

    return {N, Length, Bottom, Top};
}

void ApplyBoundaries(Field& Output, const Problem& Data)
{
    Output[0] = static_cast<double>(Data.Bottom);
    Output[Data.N - 1] = static_cast<double>(Data.Top);
}

} // namespace

void Pass(
    Field& Field_Obj,
    const Geometry& Geometry_Obj,
    const Mesh& Mesh_Obj,
    const SimulationBoundaries& Boundaries_Obj
)
{
    const Problem Data = Validate(Field_Obj, Geometry_Obj, Mesh_Obj, Boundaries_Obj);

    for (std::size_t i = 1; i < Data.N - 1; ++i)
    {
        Field_Obj[i] = static_cast<double>(Data.SteadyValue(i));
    }
    ApplyBoundaries(Field_Obj, Data);
}

void Pass(
    Field& Field_Obj,
    double Time_,
    double nu,
    const Geometry& Geometry_Obj,
    const Mesh& Mesh_Obj,
    const SimulationBoundaries& Boundaries_Obj,
    const Field& Field_0
)
{
    const Problem Data = Validate(Field_Obj, Geometry_Obj, Mesh_Obj, Boundaries_Obj);

    if (!std::isfinite(Time_) || Time_ < 0.0
        || !std::isfinite(nu) || nu < 0.0)
    {
        throw std::invalid_argument(
            "AnalyticalSolution: Time and diffusivity must be finite and nonnegative."
        );
    }
    if (Field_0.GetSize() != Data.N)
    {
        throw std::invalid_argument(
            "AnalyticalSolution: Initial field size must match the mesh size."
        );
    }
    for (std::size_t i = 0; i < Data.N; ++i)
    {
        if (!std::isfinite(Field_0[i]))
        {
            throw std::invalid_argument(
                "AnalyticalSolution: Initial field values must be finite."
            );
        }
    }

    // Preserve initial interior samples exactly when there is no evolution.
    // Fixed Dirichlet values take precedence at the two endpoints, including t=0.
    if (Time_ == 0.0 || nu == 0.0)
    {
        for (std::size_t i = 1; i < Data.N - 1; ++i)
        {
            Field_Obj[i] = Field_0[i];
        }
        ApplyBoundaries(Field_Obj, Data);
        return;
    }

    const std::size_t Intervals = Data.N - 1;
    const std::size_t NumberOfModes = Data.N - 2;
    const long double Pi = std::acos(-1.0L);
    const long double AngleStep = Pi / static_cast<long double>(Intervals);
    const long double Scale = 2.0L / static_cast<long double>(Intervals);
    std::vector<long double> Residual(Data.N, 0.0L);
    std::vector<long double> DecayedCoefficients(NumberOfModes, 0.0L);

    // Snapshot all initial samples before writing, allowing Output == Initial.
    for (std::size_t i = 1; i < Intervals; ++i)
    {
        Residual[i] = static_cast<long double>(Field_0[i]) - Data.SteadyValue(i);
    }

    // DST-I of the interior residual gives the coefficients of its sine
    // interpolant: A_k = 2/(N-1) * sum_j r_j sin(pi*k*j/(N-1)).
    // There are N-2 independent modes; mode N-1 vanishes at every mesh node.
    // This solves the continuous PDE for that interpolant, not for an unknown
    // continuous initial profile between samples. Refine the mesh to assess
    // spatial accuracy, particularly near t=0 or for nonsmooth initial data.
    for (std::size_t Mode = 1; Mode <= NumberOfModes; ++Mode)
    {
        const long double WaveNumber =
            static_cast<long double>(Mode) * Pi / Data.Length;
        const long double DecayExponent =
            static_cast<long double>(nu) * WaveNumber * WaveNumber
            * static_cast<long double>(Time_);
        const long double Decay = std::exp(-DecayExponent);

        // Underflowed modes have no representable contribution.
        if (Decay == 0.0L)
        {
            continue;
        }

        CompensatedSum Sum;
        for (std::size_t i = 1; i < Intervals; ++i)
        {
            const long double Angle = AngleStep * static_cast<long double>(Mode)
                * static_cast<long double>(i);
            Sum.Add(Residual[i] * std::sin(Angle));
        }
        DecayedCoefficients[Mode - 1] = Scale * Sum.Get() * Decay;
    }

    for (std::size_t i = 1; i < Intervals; ++i)
    {
        CompensatedSum Sum;
        for (std::size_t Mode = 1; Mode <= NumberOfModes; ++Mode)
        {
            if (DecayedCoefficients[Mode - 1] == 0.0L)
            {
                continue;
            }
            const long double Angle = AngleStep * static_cast<long double>(Mode)
                * static_cast<long double>(i);
            Sum.Add(DecayedCoefficients[Mode - 1] * std::sin(Angle));
        }
        Field_Obj[i] = static_cast<double>(Data.SteadyValue(i) + Sum.Get());
    }
    ApplyBoundaries(Field_Obj, Data);
}

} // namespace DiffusionEQ::AnalyticalSolution

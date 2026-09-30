#pragma once

#include <string>
#include <sstream>
#include <string_view>
#include "FieldName.hpp"




namespace LegendCreator
{

// ============================================================================
// Gnuplot Plot Legend Generators (e.g., "U (t = 0.003 s)", "U (Initial)")
// ============================================================================

// Overload 3: Generates clean Gnuplot legend with physical units (e.g., "U (t = 0.003 s)")
[[nodiscard]] inline std::string CreatePlotLegend(const FieldName FieldName_, const double Time_)
{
    std::ostringstream stream;
    stream << Convert_FieldName_To_String(FieldName_) << " (t = " << Time_ << " s)";
    return stream.str();
}

// Overload 4: Generates clean Gnuplot legend with string label (e.g., "U (Initial)", "U (Analytical)")
[[nodiscard]] inline std::string CreatePlotLegend(const FieldName FieldName_, const std::string_view Label_)
{
    return std::string(Convert_FieldName_To_String(FieldName_)) + " (" + std::string(Label_) + ")";
}

}
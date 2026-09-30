#pragma once

#include <string>
#include <sstream>
#include <string_view>
#include "FieldName.hpp"


namespace NameCreator
{


// ============================================================================
// File & Identifier Name Generators (e.g., "U_0.003", "U_Initial")
// ============================================================================

// Overload 1: Generates file name with numerical timestamp (e.g., "U_0.003")
[[nodiscard]] inline std::string CreateName(const FieldName FieldName_, const double Time_)
{
    std::ostringstream stream;
    stream << Convert_FieldName_To_String(FieldName_) << "_" << Time_;
    return stream.str();
}

// Overload 2: Generates file name with descriptive string label (e.g., "U_Initial", "U_Analytical")
[[nodiscard]] inline std::string CreateName(const FieldName FieldName_, const std::string_view Label_)
{
    return std::string(Convert_FieldName_To_String(FieldName_)) + "_" + std::string(Label_);
}


}

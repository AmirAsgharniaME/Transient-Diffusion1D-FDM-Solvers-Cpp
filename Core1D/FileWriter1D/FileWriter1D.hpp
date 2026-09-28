#pragma once

#include "Core1D/Field1D/Field1D.hpp"
#include "Core1D/Mesh1D/Mesh1D.hpp"
#include "Config/FieldName.hpp"
#include "Verification/AnalyticalDiffusion1D/AnalyticalDiffusion1D.hpp"
#include <filesystem>
#include <string>

#include <sstream>
#include <string_view>


namespace FileWriter1D
{

void WriteField1D(
        const std::string& FileName_,
        const Field1D& Field1D_Obj,
        const Mesh1D& Mesh1D_Obj,
        const std::filesystem::path& RelativePath);


void WriteField1D(
        const std::string& FileName_,
        const AnalyticalDiffusion1D& Field1DAnalytical_Obj,
        const Mesh1D& Mesh1D_Obj,
        const std::filesystem::path& RelativePath);

}


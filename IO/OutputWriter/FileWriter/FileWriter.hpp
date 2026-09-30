#pragma once

#include "Core/Field/Field.hpp"
#include "Core/Mesh/Mesh.hpp"
#include <filesystem>
#include <string_view>



namespace FileWriter
{

void WriteField(
        std::string_view FileName_,
        const Field& Field_Obj,
        const Mesh& Mesh_Obj,
        const std::filesystem::path& RelativePath);


}


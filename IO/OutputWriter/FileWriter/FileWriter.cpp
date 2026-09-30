#include "IO/OutputWriter/FileWriter/FileWriter.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <system_error>
#include <string_view>

namespace FileWriter
{

void WriteField(
    std::string_view FileName_,
    const Field& Field_Obj, 
    const Mesh& Mesh_Obj,
    const std::filesystem::path& RelativePath)
{
    // 1. Ensure directory exists (create_directories handles existence automatically)
    std::error_code ec;
    std::filesystem::create_directories(RelativePath, ec);
    if (ec)
    {
        std::cerr << "Error: Could not create directory: " << RelativePath.string() 
                  << " (" << ec.message() << ")\n";
        return;
    }

    // 2. Efficient path composition: zero temporary std::strings
    std::filesystem::path fullFilePath = RelativePath / FileName_;
    fullFilePath += ".dat"; // or fullFilePath.replace_extension(".dat");

    // 3. Open file stream
    std::ofstream file(fullFilePath);
    if (!file.is_open()) 
    {
        std::cerr << "Error: Failed to create " << fullFilePath.string() 
                  << " - Check folder permissions or path validity.\n";
        return;
    }

    // 4. Formatted data output
    file << std::fixed << std::setprecision(6);
    const std::size_t size = Field_Obj.GetSize();
    for (std::size_t i = 0; i < size; ++i)
    {
        file << std::left
             << std::setw(20) << Mesh_Obj[i]
             << std::setw(20) << Field_Obj[i]
             << '\n';
    }
                         
    std::cout << fullFilePath.filename().string() 
              << " Created Successfully At: " << fullFilePath.string() << '\n';
}

} // End of namespace FileWriter

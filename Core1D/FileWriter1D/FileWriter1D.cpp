#include "Core1D/FileWriter1D/FileWriter1D.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <system_error>


namespace FileWriter1D
{

void WriteField1D(
    const std::string& FileName_,
    const Field1D& Field1D_Obj, 
    const Mesh1D& Mesh1D_Obj,
    const std::filesystem::path& RelativePath)
{
    const std::string_view FileExtension = ".dat";

    std::error_code ec;
    if (!std::filesystem::exists(RelativePath, ec))
    {
        std::filesystem::create_directories(RelativePath, ec);
        if (ec)
        {
            std::cerr << "Error: Could not create directory: " << RelativePath.string() 
                      << " (" << ec.message() << ")" << std::endl;
            return;
        }
    }

    // Compose final path using filesystem operations
    std::filesystem::path fullFilePath = RelativePath / (FileName_ + std::string(FileExtension));
    std::ofstream file(fullFilePath);

    if (file.is_open()) 
    {
        file << std::fixed << std::setprecision(6);
        for (std::size_t i = 0; i < Field1D_Obj.GetSize(); ++i)
        {
            file << std::left
                 << std::setw(20) << Mesh1D_Obj[i]
                 << std::setw(20) << Field1D_Obj[i]
                 << "\n";
        }
                             
        std::cout << FileName_ << FileExtension 
                  << " Created Successfully At: " << fullFilePath.string() << std::endl;
    }
    else 
    {
        std::cerr << "Error: Failed to create " << fullFilePath.string() 
                  << " - Check folder permissions or path validity." << std::endl;
    }
}





void WriteField1D(
    const std::string& FileName_,
    const AnalyticalDiffusion1D& Field1DAnalytical_Obj,
    const Mesh1D& Mesh1D_Obj,
    const std::filesystem::path& RelativePath)
{
    const std::string_view FileExtension = ".dat";

    std::error_code ec;
    if (!std::filesystem::exists(RelativePath, ec))
    {
        std::filesystem::create_directories(RelativePath, ec);
        if (ec)
        {
            std::cerr << "Error: Could not create directory: " << RelativePath.string() 
                      << " (" << ec.message() << ")" << std::endl;
            return;
        }
    }

    // Compose final path using filesystem operations
    std::filesystem::path fullFilePath = RelativePath / (FileName_ + std::string(FileExtension));
    std::ofstream file(fullFilePath);

    if (file.is_open()) 
    {
        file << std::fixed << std::setprecision(6);
        for (std::size_t i = 0; i < Field1DAnalytical_Obj.GetSize(); ++i)
        {
            file << std::left
                 << std::setw(20) << Mesh1D_Obj[i]
                 << std::setw(20) << Field1DAnalytical_Obj[i]
                 << "\n";
        }
                             
        std::cout << FileName_ << FileExtension 
                  << " Created Successfully At: " << fullFilePath.string() << std::endl;
    }
    else 
    {
        std::cerr << "Error: Failed to create " << fullFilePath.string() 
                  << " - Check folder permissions or path validity." << std::endl;
    }
}



}//End of namespace FileWriter1D

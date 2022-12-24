#pragma once

#include <vector>
#include <string>
#include <filesystem>

namespace io
{
    enum class Endianness
    {
        Little,
        Big
    };

    std::string read_text_file(const std::string& filepath);
    std::vector<std::filesystem::path> get_files_recursive(const std::string& dir_path);

    void create_directories_for_file(const std::string& filepath);
}
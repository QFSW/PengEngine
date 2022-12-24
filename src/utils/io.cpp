#include "io.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace io
{
    namespace fs = std::filesystem;

    std::string read_text_file(const std::string& filepath)
    {
        const std::ifstream file(filepath);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file " + filepath);
        }

        std::stringstream buf;
        buf << file.rdbuf();

        return buf.str();
    }

    std::vector<fs::path> get_files_recursive(const std::string& dir_path)
    {
        std::vector<fs::path> files;
        for (const auto& entry : fs::recursive_directory_iterator(dir_path))
        {
            if (entry.is_regular_file())
            {
                files.push_back(entry.path());
            }
        }

        return files;
    }

    void create_directories_for_file(const std::string& filepath)
    {
        fs::path path = filepath;
        fs::path dir = path.parent_path();

        if (!fs::exists(dir) || !fs::is_directory(dir))
        {
            fs::create_directories(dir);
        }
    }
}

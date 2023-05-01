#include "archive.h"

#include <fstream>
#include <filesystem>

#include <utils/check.h>
#include <profiling/scoped_event.h>

Archive Archive::from_disk(const std::string& path)
{
    SCOPED_EVENT("Loading archive", path.c_str());

    std::ifstream file(path);
    check(file.is_open());

    Archive archive;
    archive.path = path;
    archive.json_def = nlohmann::json::parse(file);

    {
        namespace fs = std::filesystem;
        archive.name = archive.read_or<std::string>(
            "name", fs::path(path).filename().string()
        );
    }

    return archive;
}
#pragma once

#include <string>

#include "raw_mesh_data.h"

namespace rendering
{
    class MeshDecoder
    {
    public:
        static RawMeshData load_file(const std::string& path);

    private:
        static RawMeshData load_obj(const std::string& path);
    };
}
#pragma once

#include <vector>

#include "vertex.h"

namespace rendering
{
    struct RawMeshData
    {
        std::vector<Vertex> vertices;
        std::vector<math::Vector3u> triangles;

        bool corrupt = false;

        void check_valid() const;

        static RawMeshData corrupt_data();
    };
}

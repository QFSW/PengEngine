#include "utils.h"

#include <common/common.h>

using namespace rendering;
using namespace math;

std::tuple<std::vector<Vertex>, std::vector<Vector3u>> rendering::subdivide(
    const std::vector<Vertex>& vertices,
    const std::vector<Vector3u>& indices
)
{
    std::vector<Vertex> vertices_subdivided;
    std::vector<Vector3u> indices_subdivided;

    common::unordered_map<Vector3f, uint32_t> vertex_index_cache;
    auto get_index = [&](const Vertex& vertex) -> uint32_t
    {
        if (const auto it = vertex_index_cache.find(vertex.position); it != vertex_index_cache.end())
        {
            return it->second;
        }

        const uint32_t index = static_cast<uint32_t>(vertices_subdivided.size());
        vertices_subdivided.push_back(vertex);

        return vertex_index_cache[vertex.position] = index;
    };

    for (const Vector3u& tri : indices)
    {
        const Vertex& v0 = vertices[tri.x];
        const Vertex& v1 = vertices[tri.y];
        const Vertex& v2 = vertices[tri.z];

	    const uint32_t tri_a = get_index(v0);
        const uint32_t tri_b = get_index(v1);
        const uint32_t tri_c = get_index(v2);

        const uint32_t tri_ab = get_index(Vertex::subdivide(v0, v1));
        const uint32_t tri_ac = get_index(Vertex::subdivide(v0, v2));
        const uint32_t tri_bc = get_index(Vertex::subdivide(v1, v2));

        indices_subdivided.emplace_back(tri_a, tri_ab, tri_ac);
        indices_subdivided.emplace_back(tri_ac, tri_bc, tri_c);
        indices_subdivided.emplace_back(tri_ab, tri_bc, tri_ac);
        indices_subdivided.emplace_back(tri_ab, tri_b, tri_bc);
    }

    return std::make_tuple(vertices_subdivided, indices_subdivided);
}

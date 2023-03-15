#include "utils.h"

#include <unordered_map>
#include <unordered_set>

using namespace rendering;
using namespace math;

std::tuple<std::vector<Vertex>, std::vector<Vector3u>> rendering::subdivide(
    const std::vector<Vertex>& vertices,
    const std::vector<Vector3u>& indices
)
{
    // Copy original vertex buffer so that we can reuse existing indices
    // for existing vertices to make preserving seams easier
    std::vector<Vertex> vertices_subdivided = vertices;
    std::vector<Vector3u> indices_subdivided;

    const uint32_t num_vertices = static_cast<uint32_t>(vertices.size());

    // Generate a list of which indices correspond to vertices on a seam
    // so that we can preserve seams when subdividing
    std::unordered_set<uint32_t> seam_indices;
    std::unordered_map<Vector3f, size_t> vertex_hits;

    // Initialize vertex hits to zero
    for (const Vertex& vertex : vertices)
    {
        vertex_hits[vertex.position] = 0;
    }

    // Count hits for each vertex
    for (const Vertex& vertex : vertices)
    {
        vertex_hits[vertex.position]++;
    }

    // All indices that correspond to vertices with >=2 vertex hits are seam indices
    for (uint32_t index = 0; index < num_vertices; index++)
    {
        const Vertex& vertex = vertices[index];
        if (vertex_hits[vertex.position] >= 2)
        {
            seam_indices.insert(index);
        }
    }

    // Determines if an edge of two indices is considered a seam
    auto is_seam = [&](uint32_t a, uint32_t b)
    {
        return seam_indices.contains(a) && seam_indices.contains(b);
    };

    // Gets the index for the provided vertex
    // If the given vertex is a seam, then a new index will always be created
    // otherwise, it will attempt to pool with vertices that already exist
    std::unordered_map<Vector3f, uint32_t> vertex_index_cache;
    auto get_index = [&](const Vertex& vertex, bool in_seam) -> uint32_t
    {
        if (!in_seam)
        {
            if (const auto it = vertex_index_cache.find(vertex.position); it != vertex_index_cache.end())
            {
                return it->second;
            }
        }

        const uint32_t index = static_cast<uint32_t>(vertices_subdivided.size());
        vertices_subdivided.push_back(vertex);

        if (!in_seam)
        {
            vertex_index_cache[vertex.position] = index;
        }

        return index;
    };

    for (const Vector3u& tri : indices)
    {
        // Use original indices directly to preserve seams
        const uint32_t i0 = tri.x;
        const uint32_t i1 = tri.y;
        const uint32_t i2 = tri.z;

        const Vertex& v0 = vertices[i0];
        const Vertex& v1 = vertices[i1];
        const Vertex& v2 = vertices[i2];

        const uint32_t i01 = get_index(Vertex::subdivide(v0, v1), is_seam(i0, i1));
        const uint32_t i02 = get_index(Vertex::subdivide(v0, v2), is_seam(i0, i2));
        const uint32_t i12 = get_index(Vertex::subdivide(v1, v2), is_seam(i1, i2));

        indices_subdivided.emplace_back(i0, i01, i02);
        indices_subdivided.emplace_back(i02, i12, i2);
        indices_subdivided.emplace_back(i01, i12, i02);
        indices_subdivided.emplace_back(i01, i1, i12);
    }

    return std::make_tuple(vertices_subdivided, indices_subdivided);
}

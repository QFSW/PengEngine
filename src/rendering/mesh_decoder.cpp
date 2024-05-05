#include "mesh_decoder.h"

#include <fstream>
#include <filesystem>
#include <unordered_map>

#include <core/logger.h>
#include <profiling/scoped_event.h>

using namespace rendering;
using namespace math;

// DECODE_ERROR begin
#define DECODE_ERROR(...)               \
    Logger::error(__VA_ARGS__);         \
    return RawMeshData::corrupt_data()  \
// DECODE_ERROR end

// DECODE_CHECK begin
#define DECODE_CHECK(expr, fmt, ...)    \
    if (!(expr))                        \
    {                                   \
        DECODE_ERROR(fmt, __VA_ARGS__); \
    } do {} while (0)
// DECODE_CHECK end

RawMeshData MeshDecoder::load_file(const std::string& path)
{
    SCOPED_EVENT("MeshDecoder - loading mesh file", path.c_str());
    Logger::log("Loading mesh file '%s'", path.c_str());

    namespace fs = std::filesystem;
    const fs::path ext = fs::path(path).extension();

    if (ext == ".obj")
    {
        return load_obj(path);
    }

    DECODE_ERROR(
        "Cannot decode mesh file '%s': unsupported extension %s",
        path.c_str(), ext.string().c_str()
    );
}

RawMeshData MeshDecoder::load_obj(const std::string& path)
{
    SCOPED_EVENT("MeshDecoder - loading OBJ file");

    std::ifstream file(path);
    DECODE_CHECK(file.is_open(), "Cannot open file %s", path.c_str());

    std::string line;
    int32_t line_number = 0;
    std::vector<Vector3f> vertex_positions;
    std::vector<Vector3f> vertex_normals;
    std::vector<Vector2f> uv_coords;
    std::unordered_map<std::string, uint32_t> vertex_cache;

    RawMeshData raw_data;

    while (std::getline(file, line))
    {
        line_number++;

        static thread_local std::vector<std::string> tokens;
        tokens.clear();
        strtools::split(line, ' ', std::back_inserter(tokens));

        // Skip empty lines
        if (tokens.empty())
        {
            continue;
        }

        // Vertex positions
        if (tokens[0] == "v")
        {
            DECODE_CHECK(tokens.size() == 4,
                "Invalid OBJ file - invalid vertex\n%d: %s",
                line_number, line.c_str()
            );

            vertex_positions.emplace_back(
                std::stof(tokens[1]),
                std::stof(tokens[2]),
                std::stof(tokens[3])
            );
        }

        // Vertex normals
        if (tokens[0] == "vn")
        {
            DECODE_CHECK(tokens.size() == 4,
                "Invalid OBJ file - invalid vertex normal\n%d: %s",
                line_number, line.c_str()
            );

            vertex_normals.emplace_back(
                std::stof(tokens[1]),
                std::stof(tokens[2]),
                std::stof(tokens[3])
            );
        }

        // UV coords
        if (tokens[0] == "vt")
        {
            DECODE_CHECK(tokens.size() == 3,
                "Invalid OBJ file - invalid UV coord\n%d: %s",
                line_number, line.c_str()
            );

            uv_coords.emplace_back(
                std::stof(tokens[1]),
                std::stof(tokens[2])
            );
        }

        // Faces
        if (tokens[0] == "f")
        {
            DECODE_CHECK(tokens.size() == 4,
                "Invalid OBJ file - non triangular face enountered which is not supported\n%d: %s",
                line_number, line.c_str()
            );

            uint32_t triangle[3];
            for (uint8_t i = 0; i < 3; i++)
            {
                const std::string& vertex_token = tokens[1 + i];
                uint32_t vertex_index;

                if (const auto it = vertex_cache.find(vertex_token); it != vertex_cache.end())
                {
                    vertex_index = it->second;
                }
                else
                {
                    // Decode token
                    static thread_local std::vector<std::string> inner_tokens;
                    inner_tokens.clear();
                    strtools::split(vertex_token, '/', std::back_inserter(inner_tokens));

                    DECODE_CHECK(inner_tokens.size() == 3,
                        "Invalid OBJ file - vertex '%s' does not use the v/vt/vn format\n%d: %s",
                        line_number, vertex_token.c_str(), line.c_str()
                    );

                    // OBJ files use 1 indexing instead of 0 indexing
                    const uint32_t position_index = std::stoi(inner_tokens[0]);
                    const uint32_t tex_coord_index = std::stoi(inner_tokens[1]);
                    const uint32_t normal_index = std::stoi(inner_tokens[2]);

                    DECODE_CHECK(position_index <= vertex_positions.size(),
                        "Invalid OBJ file - vertex '%s' has invalid position index %d\n%d: %s",
                        vertex_token.c_str(), position_index, line_number, line.c_str()
                    );

                    DECODE_CHECK(tex_coord_index <= uv_coords.size(),
                        "Invalid OBJ file - vertex '%s' has invalid tex coord index %d\n%d: %s",
                        vertex_token.c_str(), tex_coord_index, line_number, line.c_str()
                    );

                    DECODE_CHECK(normal_index <= vertex_normals.size(),
                        "Invalid OBJ file - vertex '%s' has invalid normal index %d\n%d: %s",
                        vertex_token.c_str(), normal_index, line_number, line.c_str()
                    );

                    const Vertex vertex(
                        vertex_positions[position_index - 1],
                        vertex_normals[normal_index - 1],
                        uv_coords[tex_coord_index - 1]
                    );

                    vertex_index = static_cast<uint32_t>(raw_data.vertices.size());
                    vertex_cache[vertex_token] = vertex_index;

                    raw_data.vertices.push_back(vertex);
                }

                triangle[i] = vertex_index;
            }

            raw_data.triangles.emplace_back(
                triangle[0],
                triangle[1],
                triangle[2]
            );
        }
    }

    return raw_data;
}

#pragma once

#include <vector>
#include <unordered_map>

#include <memory/shared_ref.h>
#include <utils/hash_helpers.h>

namespace rendering
{
    class Mesh;
    class Shader;
    class Material;

    // Draw calls aggregated by the mesh, only differing in the uniforms applied
    // It is illegal for the materials to use different underlying shaders
    struct MeshDrawTree
    {
        size_t index;
        peng::shared_ref<const Mesh> mesh;
        std::vector<peng::shared_ref<Material>> materials;
    };

    // Draw calls aggregated by the shader. Each draw may differ by mesh and uniforms
    struct ShaderDrawTree
    {
        size_t index;
        peng::shared_ref<const Shader> shader;
        std::vector<MeshDrawTree> mesh_draws;
    };

    // Tree of draw calls aggregated by shader, then mesh, the uniforms
    // This allows all draw calls to be executed with minimal state switches
    class DrawCallTree
    {
    public:
        DrawCallTree(std::vector<struct DrawCall>&& draw_calls);

        void execute() const;

    private:
        ShaderDrawTree& find_add_shader_draw(const peng::shared_ref<const Shader>& shader);

        MeshDrawTree& find_add_mesh_draw(
            const peng::shared_ref<const Shader>& shader,
            const peng::shared_ref<const Mesh>& mesh
        );

        std::vector<ShaderDrawTree> _shader_draws;

        std::unordered_map<peng::shared_ref<const Shader>, size_t> _shader_draw_indices;

        // Maps from a (shader, mesh) key to a (shader_draw index, mesh_draw sub index) value
        std::unordered_map<
            std::tuple<
            peng::shared_ref<const Shader>,
            peng::shared_ref<const Mesh>
            >,
            std::tuple<size_t, size_t>
        > _mesh_draw_indices;
    };
}

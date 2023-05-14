#pragma once

#include <string>
#include <GL/glew.h>

#include <memory/shared_ref.h>

#include "raw_mesh_data.h"

struct Archive;

namespace rendering
{
    // TODO: generate bounds for the mesh to be used in culling
    class Mesh
    {
    public:
        Mesh(std::string&& name, RawMeshData&& raw_data);
        Mesh(const std::string& name, const RawMeshData& raw_data);
        Mesh(const std::string& name, const std::string& mesh_path);

        Mesh(const Mesh&) = delete;
        Mesh(Mesh&&) = delete;
        ~Mesh();

        static peng::shared_ref<Mesh> load_asset(const Archive& archive);

        // Renders the mesh
        // A shader/material must already be in use before calling this
        void render() const;

        void bind() const;
        void unbind() const;
        void draw() const;
        void draw_instanced(int32_t num) const;

        [[nodiscard]] const std::string& name() const noexcept;
        [[nodiscard]] int32_t num_triangles() const noexcept;

    private:
        std::string _name;
        RawMeshData _raw_data;
        GLuint _num_indices;

        GLuint _ebo;
        GLuint _vbo;
        GLuint _vao;
    };
}

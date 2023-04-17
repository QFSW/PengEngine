#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

#include "vertex.h"

namespace rendering
{
    // TODO: generate bounds for the mesh to be used in culling
    class Mesh
    {
    public:
        Mesh(
            std::string&& name,
            std::vector<Vertex>&& vertices,
            std::vector<math::Vector3u>&& indices
        );

        Mesh(
            const std::string& name,
            const std::vector<Vertex>& vertices,
            const std::vector<math::Vector3u>& indices
        );

        Mesh(const Mesh&) = delete;
        Mesh(Mesh&&) = delete;
        ~Mesh();

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
        std::vector<Vertex> _vertex_buffer;
        std::vector<math::Vector3u> _index_buffer;

        GLuint _num_indices;

        GLuint _ebo;
        GLuint _vbo;
        GLuint _vao;
    };
}
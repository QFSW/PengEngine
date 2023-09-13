#include "mesh.h"

#include <utils/utils.h>
#include <utils/check.h>
#include <utils/vectools.h>
#include <core/archive.h>
#include <core/logger.h>
#include <memory/gc.h>
#include <profiling/scoped_event.h>

#include "mesh_decoder.h"
#include "gl_wrapper.h"

using namespace rendering;
using namespace math;

Mesh::Mesh(std::string&& name, RawMeshData&& raw_data)
    : _name(std::move(name))
    , _raw_data(std::move(raw_data))
    , _num_indices(static_cast<GLuint>(_raw_data.triangles.size() * 3))
{
    SCOPED_EVENT("Building mesh", _name.c_str());
    Logger::log("Building mesh '%s'", _name.c_str());

    _raw_data.check_valid();

    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
    glGenVertexArrays(1, &_vao);

    glBindVertexArray(_vao);
    object_label(GL_VERTEX_ARRAY, _vao, -1, _name.c_str());

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vectools::buffer_size(_raw_data.vertices), _raw_data.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vectools::buffer_size(_raw_data.triangles), _raw_data.triangles.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(3);
}

Mesh::Mesh(const std::string& name, const RawMeshData& raw_data)
    : Mesh(
        utils::copy(name),
        utils::copy(raw_data)
    )
{ }

Mesh::Mesh(const std::string& name, const std::string& mesh_path)
    : Mesh(
        name,
        MeshDecoder::load_file(mesh_path)
    )
{ }

Mesh::~Mesh()
{
    SCOPED_EVENT("Destroying mesh", _name.c_str());
    Logger::log("Destroying mesh '%s'", _name.c_str());

    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao);
}

peng::shared_ref<Mesh> Mesh::load_asset(const Archive& archive)
{
    const std::string mesh_path = archive.read<std::string>("mesh");
    return memory::GC::alloc<Mesh>(archive.name, mesh_path);
}

void Mesh::render() const
{
    bind();
    draw();
    unbind();
}

void Mesh::bind() const
{
    glBindVertexArray(_vao);
}

void Mesh::unbind() const
{
    glBindVertexArray(0);
}

void Mesh::draw() const
{
    glDrawElements(GL_TRIANGLES, _num_indices, GL_UNSIGNED_INT, nullptr);
}

void Mesh::draw_instanced(int32_t num) const
{
    check(num >= 0);
    glDrawElementsInstanced(GL_TRIANGLES, _num_indices, GL_UNSIGNED_INT, nullptr, num);
}

const std::string& Mesh::name() const noexcept
{
    return _name;
}

int32_t Mesh::num_triangles() const noexcept
{
    return static_cast<int32_t>(_raw_data.triangles.size());
}


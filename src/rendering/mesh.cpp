#include "mesh.h"

#include <utils/utils.h>
#include <utils/strtools.h>
#include <utils/vectools.h>
#include <core/logger.h>

using namespace rendering;
using namespace math;

Mesh::Mesh(
	const std::string& name,
	const std::vector<Vector3f>& vertices,
	const std::vector<Vector3u>& indices,
	const std::vector<Vector3f>& colors,
	const std::vector<Vector2f>& tex_coords
)
	: _name(name)
	, _index_buffer(indices)
	, _num_indices(static_cast<GLuint>(indices.size() * 3))
	, _valid(true)
{
	if constexpr (Logger::enabled())
	{
		Logger::get().logf(LogVerbosity::Log, "Building mesh '%s'", _name.c_str());

		if (!colors.empty() && colors.size() != vertices.size())
		{
			Logger::get().logf(LogVerbosity::Warning,
				"Mesh '%s' has %d vertices but %d vertex colors",
				_name.c_str(), vertices.size(), colors.size()
			);
		}

		if (!tex_coords.empty() && tex_coords.size() != vertices.size())
		{
			Logger::get().logf(LogVerbosity::Warning,
				"Mesh '%s' has %d vertices but %d texture coordinates",
				_name.c_str(), vertices.size(), tex_coords.size()
			);
		}
	}

	_vertex_buffer.resize(vertices.size(), Vertex(Vector3f::zero(), Vector3f::one(), Vector2f::zero()));
	for (size_t vert_index = 0; vert_index < vertices.size(); vert_index++)
	{
		_vertex_buffer[vert_index].position = vertices[vert_index];

		if (vert_index < colors.size())
		{
			_vertex_buffer[vert_index].color = colors[vert_index];
		}

		if (vert_index < tex_coords.size())
		{
			_vertex_buffer[vert_index].tex_coord = tex_coords[vert_index];
		}
	}

	allocate_opengl();
}

Mesh::Mesh(const Mesh& other)
	: _name(other._name)
	, _vertex_buffer(other._vertex_buffer)
	, _index_buffer(other._index_buffer)
	, _num_indices(other._num_indices)
	, _valid(true)
{
	Logger::get().logf(LogVerbosity::Log, "Copying mesh '%s'", _name.c_str());
	allocate_opengl();
}

Mesh::Mesh(Mesh&& other) noexcept
	: _name(std::move(other._name))
    , _vertex_buffer(std::move(other._vertex_buffer))
    , _index_buffer(std::move(other._index_buffer))
    , _num_indices(other._num_indices)
    , _ebo(other._ebo)
    , _vbo(other._vbo)
    , _vao(other._vao)
    , _valid(true)
{
	other._ebo = 0;
	other._vbo = 0;
	other._vao = 0;
	other._valid = false;
}

Mesh::~Mesh()
{
	if (_valid)
	{
		Logger::get().logf(LogVerbosity::Log, "Destroying mesh '%s'", _name.c_str());

		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ebo);
		glDeleteVertexArrays(1, &_vao);
	}
}

void Mesh::render() const
{
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _num_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::allocate_opengl()
{
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
	glGenVertexArrays(1, &_vao);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vectools::buffer_size(_vertex_buffer), _vertex_buffer.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vectools::buffer_size(_index_buffer), _index_buffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
	glEnableVertexAttribArray(2);
}

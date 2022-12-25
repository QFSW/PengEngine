#include "mesh.h"

#include <utils/utils.h>
#include <utils/strtools.h>
#include <utils/vectools.h>

using namespace rendering;
using namespace math;

Mesh::Mesh(
	const std::vector<Vector3f>& vertices,
	const std::vector<Vector3u>& indices,
	const std::vector<Vector3f>& colors,
	const std::vector<Vector2f>& tex_coords
)
	: _index_buffer(indices)
{
	_vertex_buffer.resize(8 * vertices.size());
	for (size_t vert_index = 0; vert_index < vertices.size(); vert_index++)
	{
		const Vector3f& vertex = vertices[vert_index];

		const Vector3f vert_color =
			vert_index < colors.size()
			? colors[vert_index]
			: Vector3f(1, 1, 1);

		const Vector2f tex_coord =
			vert_index < tex_coords.size()
			? tex_coords[vert_index]
			: Vector2f(0, 0);

		_vertex_buffer[vert_index * 8 + 0] = vertex.x;
		_vertex_buffer[vert_index * 8 + 1] = vertex.y;
		_vertex_buffer[vert_index * 8 + 2] = vertex.z;
		_vertex_buffer[vert_index * 8 + 3] = vert_color.x;
		_vertex_buffer[vert_index * 8 + 4] = vert_color.y;
		_vertex_buffer[vert_index * 8 + 5] = vert_color.z;
		_vertex_buffer[vert_index * 8 + 6] = tex_coord.x;
		_vertex_buffer[vert_index * 8 + 7] = tex_coord.y;
	}

	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
	glGenVertexArrays(1, &_vao);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vectools::buffer_size(_vertex_buffer), _vertex_buffer.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vectools::buffer_size(_index_buffer), _index_buffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao);
}

void Mesh::render() const
{
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
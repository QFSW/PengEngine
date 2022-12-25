#include "mesh.h"

#include <utils/utils.h>
#include <utils/strtools.h>
#include <utils/vectools.h>

using namespace rendering;
using namespace math;

Mesh::Mesh(
	const std::vector<Vector3f>& vertices,
	const std::vector<Vector3u>& indices,
	const std::vector<Vector3f>& colors
)
	: _indexBuffer(indices)
{
	_vertexBuffer.resize(2 * vertices.size());
	for (size_t vert_index = 0; vert_index < vertices.size(); vert_index++)
	{
		const Vector3f vert_color =
			vert_index < colors.size()
			? colors[vert_index]
			: Vector3f(1, 1, 1);

		_vertexBuffer[vert_index * 2 + 0] = vertices[vert_index];
		_vertexBuffer[vert_index * 2 + 1] = vert_color;
	}

	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
	glGenVertexArrays(1, &_vao);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vectools::buffer_size(_vertexBuffer), _vertexBuffer.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vectools::buffer_size(_indexBuffer), _indexBuffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(Vector3f), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(Vector3f), (void*)sizeof(Vector3f));
	glEnableVertexAttribArray(1);
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
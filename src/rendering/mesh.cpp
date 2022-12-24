#include "mesh.h"

#include <utils/utils.h>
#include <utils/strtools.h>
#include <utils/vectools.h>

using namespace rendering;
using namespace math;

Mesh::Mesh(
	const std::vector<math::Vector3f>& vertices,
	const std::vector<math::Vector3u>& indices,
	const std::vector<math::Vector3f>& colors
)
	: _indexBuffer(indices)
{
	_vertexBuffer.resize(2 * vertices.size());
	for (size_t vertIndex = 0; vertIndex < vertices.size(); vertIndex++)
	{
		const Vector3f vertColor =
			vertIndex < colors.size()
			? colors[vertIndex]
			: Vector3f(1, 1, 1);

		_vertexBuffer[vertIndex * 2 + 0] = vertices[vertIndex];
		_vertexBuffer[vertIndex * 2 + 1] = vertColor;
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
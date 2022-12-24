#pragma once

#include <vector>

#include <GL/glew.h>
#include <math/vector3.h>

namespace rendering
{
	class Mesh
	{
	public:
		Mesh(
			const std::vector<math::Vector3f>& vertices,
			const std::vector<math::Vector3u>& indices,
			const std::vector<math::Vector3f>& colors
		);

		~Mesh();

		void render() const;

	private:
		std::vector<math::Vector3f> _vertexBuffer;
		std::vector<math::Vector3u> _indexBuffer;

		GLuint _ebo;
		GLuint _vbo;
		GLuint _vao;
	};
}
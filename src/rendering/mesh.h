#pragma once

#include <vector>

#include <GL/glew.h>

namespace rendering
{
	class Mesh
	{
	public:
		Mesh(std::vector<float>&& vertices, std::vector<GLuint>&& indices);
		Mesh(const std::vector<float>& vertices, const std::vector<GLuint>& indices);
		~Mesh();

		void render() const;

	private:
		std::vector<float> _vertices;
		std::vector<GLuint> _indices;

		GLuint _ebo;
		GLuint _vbo;
		GLuint _vao;
	};
}
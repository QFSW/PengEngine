#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

#include "vertex.h"

namespace rendering
{
	class Mesh
	{
	public:
		Mesh(
			const std::string& name,
			const std::vector<math::Vector3f>& vertices,
			const std::vector<math::Vector3u>& indices,
			const std::vector<math::Vector3f>& colors,
			const std::vector<math::Vector2f>& tex_coords
		);

		Mesh(const Mesh& other);
		Mesh(Mesh&& other) noexcept;
		~Mesh();

		void render() const;

	private:
		void allocate_opengl();

		std::string _name;
		std::vector<Vertex> _vertex_buffer;
		std::vector<math::Vector3u> _index_buffer;

		GLuint _num_indices;

		GLuint _ebo = 0;
		GLuint _vbo = 0;
		GLuint _vao = 0;
		bool _valid = false;
	};
}
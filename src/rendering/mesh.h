#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <math/vector3.h>

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

		Mesh(const Mesh&) = delete;
		Mesh(Mesh&&) = delete;
		~Mesh();

		void render() const;

	private:
		std::string _name;
		std::vector<float> _vertex_buffer;
		std::vector<math::Vector3u> _index_buffer;

		GLuint _num_indices;

		GLuint _ebo;
		GLuint _vbo;
		GLuint _vao;
	};
}
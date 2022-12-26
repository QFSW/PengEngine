#pragma once

#include <string>

#include <math/vector2.h>
#include <GL/glew.h>

namespace rendering
{
	class Texture
	{
	public:
		Texture(const std::string& name, const std::string& texture_path);
		Texture(const Texture&) = delete;
		Texture(Texture&&) = delete;
		~Texture();

		void use(GLenum slot) const;

	private:
		std::string _name;
		GLuint _tex;

		math::Vector2i _resolution;
		int32_t _num_channels;
	};
}
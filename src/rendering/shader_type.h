#pragma once

#include <ostream>

#include <GL/glew.h>

namespace rendering
{
	enum class ShaderType
	{
		vertex,
		fragment
	};

	GLenum to_opengl(ShaderType shader_type);
	std::ostream& operator<<(std::ostream& os, ShaderType shader_type);
}

#include "shader_type.h"

using namespace rendering;

GLenum rendering::to_opengl(ShaderType shader_type)
{
	switch (shader_type)
	{
		case ShaderType::fragment: return GL_FRAGMENT_SHADER;
		case ShaderType::vertex: return GL_VERTEX_SHADER;
		default: return GL_INVALID_ENUM;
	}
}

std::ostream& rendering::operator<<(std::ostream& os, ShaderType shader_type)
{
	switch (shader_type)
	{
		case ShaderType::fragment: os << "fragment"; break;
		case ShaderType::vertex: os << "vertex"; break;
		default: os << "???"; break;
	}

	return os;
}

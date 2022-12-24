#pragma once

#include <string>

#include <GL/glew.h>

namespace rendering
{
	class Shader
	{
	public:
		Shader(const std::string& vertShaderPath, const std::string& fragShaderPath);
		~Shader();

		void use() const;
		GLint get_uniform_location(const std::string& name) const;

	private:
		void validate_shader_compile(GLuint shader);
		void validate_shader_link(GLuint shader);

		GLuint _shaderProg;
	};
}
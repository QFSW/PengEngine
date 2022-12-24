#pragma once

#include <string>

#include <GL/glew.h>

namespace rendering
{
	class Shader
	{
	public:
		Shader(const std::string& vertShaderSrc, const std::string& fragShaderSrc);
		~Shader();

		void use() const;

	private:
		void validate_shader_compile(GLuint shader);
		void validate_shader_link(GLuint shader);

		GLuint _shaderProg;
	};
}
#include "shader.h"

#include <utils/io.h>

using namespace rendering;

Shader::Shader(const std::string& vertShaderPath, const std::string& fragShaderPath)
{
	const std::string vertShaderSrc = io::read_text_file(vertShaderPath);
	const std::string fragShaderSrc = io::read_text_file(fragShaderPath);

	const char* shaderSrc;

	const GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	shaderSrc = vertShaderSrc.c_str();
	glShaderSource(vertShader, 1, &shaderSrc, NULL);
	glCompileShader(vertShader);
	validate_shader_compile(vertShader);

	const GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderSrc = fragShaderSrc.c_str();
	glShaderSource(fragShader, 1, &shaderSrc, NULL);
	glCompileShader(fragShader);
	validate_shader_compile(fragShader);

	_shaderProg = glCreateProgram();
	glAttachShader(_shaderProg, vertShader);
	glAttachShader(_shaderProg, fragShader);
	glLinkProgram(_shaderProg);
	validate_shader_link(_shaderProg);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

Shader::~Shader()
{
	glDeleteProgram(_shaderProg);
}

void Shader::use() const
{
	glUseProgram(_shaderProg);
}

GLint Shader::get_uniform_location(const std::string& name) const
{
	return glGetUniformLocation(_shaderProg, name.c_str());
}

void Shader::validate_shader_compile(GLuint shader)
{
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("ERROR::SHADER::COMPILATION_FAILED\n%s", infoLog);
	}
}

void Shader::validate_shader_link(GLuint shader)
{
	GLint success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shader, 512, NULL, infoLog);
		printf("ERROR::SHADER::LINK_FAILED\n%s", infoLog);
	}
}
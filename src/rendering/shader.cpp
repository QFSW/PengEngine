#include "shader.h"

#include <core/logger.h>
#include <utils/io.h>

using namespace rendering;

Shader::Shader(const std::string& vert_shader_path, const std::string& frag_shader_path)
{
	Logger::get().logf(LogVerbosity::Log, "Loading vertex shader '%s'", vert_shader_path.c_str());
	const std::string vert_shader_src = io::read_text_file(vert_shader_path);

	Logger::get().logf(LogVerbosity::Log, "Loading fragment shader '%s'", frag_shader_path.c_str());
	const std::string frag_shader_src = io::read_text_file(frag_shader_path);

	const char* shader_src;

	Logger::get().log(LogVerbosity::Log, "Compiling vertex shader");
	const GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	shader_src = vert_shader_src.c_str();
	glShaderSource(vert_shader, 1, &shader_src, nullptr);
	glCompileShader(vert_shader);
	validate_shader_compile(vert_shader);

	Logger::get().log(LogVerbosity::Log, "Compiling fragment shader");
	const GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	shader_src = frag_shader_src.c_str();
	glShaderSource(frag_shader, 1, &shader_src, nullptr);
	glCompileShader(frag_shader);
	validate_shader_compile(frag_shader);

	Logger::get().log(LogVerbosity::Log, "Linking shader program");
	_shader_prog = glCreateProgram();
	glAttachShader(_shader_prog, vert_shader);
	glAttachShader(_shader_prog, frag_shader);
	glLinkProgram(_shader_prog);
	validate_shader_link(_shader_prog);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);
}

Shader::~Shader()
{
	glDeleteProgram(_shader_prog);
}

void Shader::use() const
{
	glUseProgram(_shader_prog);
}

GLint Shader::get_uniform_location(const std::string& name) const
{
	return glGetUniformLocation(_shader_prog, name.c_str());
}

void Shader::validate_shader_compile(GLuint shader)
{
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char info_log[512];
		glGetShaderInfoLog(shader, 512, nullptr, info_log);
		Logger::get().log(LogVerbosity::Error, info_log);
	}
}

void Shader::validate_shader_link(GLuint shader)
{
	GLint success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);

	if (!success)
	{
		char info_log[512];
		glGetProgramInfoLog(shader, 512, nullptr, info_log);
		Logger::get().log(LogVerbosity::Error, info_log);
	}
}
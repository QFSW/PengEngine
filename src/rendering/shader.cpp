#include "shader.h"

#include <cassert>

#include <memory/weak_ptr.h>
#include <core/logger.h>
#include <utils/utils.h>
#include <utils/io.h>

using namespace rendering;

Shader::Shader(
	const std::string& name,
	const std::string& vert_shader_path,
	const std::string& frag_shader_path
)
	: Shader(utils::copy(name), vert_shader_path, frag_shader_path)
{ }

Shader::Shader(
	std::string&& name,
	const std::string& vert_shader_path,
	const std::string& frag_shader_path
)
	: _name(std::move(name))
	, _broken(false)
{
	Logger::get().logf(LogSeverity::log, "Building shader '%s'", _name.c_str());
	Logger::get().logf(LogSeverity::log, "Loading vertex shader '%s'", vert_shader_path.c_str());
	const std::string vert_shader_src = io::read_text_file(vert_shader_path);

	Logger::get().logf(LogSeverity::log, "Loading fragment shader '%s'", frag_shader_path.c_str());
	const std::string frag_shader_src = io::read_text_file(frag_shader_path);

	const char* shader_src;

	Logger::get().log(LogSeverity::log, "Compiling vertex shader");
	const GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	shader_src = vert_shader_src.c_str();
	glShaderSource(vert_shader, 1, &shader_src, nullptr);
	glCompileShader(vert_shader);
	_broken |= !validate_shader_compile(vert_shader);

	Logger::get().log(LogSeverity::log, "Compiling fragment shader");
	const GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	shader_src = frag_shader_src.c_str();
	glShaderSource(frag_shader, 1, &shader_src, nullptr);
	glCompileShader(frag_shader);
	_broken |= !validate_shader_compile(frag_shader);

	Logger::get().log(LogSeverity::log, "Linking shader program");
	_shader_prog = glCreateProgram();
	glAttachShader(_shader_prog, vert_shader);
	glAttachShader(_shader_prog, frag_shader);
	glLinkProgram(_shader_prog);
	_broken |= !validate_shader_link(_shader_prog);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);
}

Shader::~Shader()
{
	Logger::get().logf(LogSeverity::log, "Destroying shader '%s'", _name.c_str());
	glDeleteProgram(_shader_prog);
}

peng::shared_ref<const Shader> Shader::fallback()
{
	static peng::weak_ptr<const Shader> weak_fallback;
	if (const peng::shared_ptr<const Shader> fallback_locked = weak_fallback.lock())
	{
		return fallback_locked.to_shared_ref();
	}

	const peng::shared_ref shader = peng::make_shared<Shader>(
		"Fallback",
		"resources/shaders/core/projection.vert",
		"resources/shaders/core/fallback.frag"
	);

	assert(!shader->broken());

	weak_fallback = shader;
	return shader;
}

void Shader::use() const
{
	// TODO: either error or assert that shader is not broken
	glUseProgram(_shader_prog);
}

const std::string& Shader::name() const noexcept
{
	return _name;
}

bool Shader::broken() const noexcept
{
	return _broken;
}

GLint Shader::get_uniform_location(const std::string& name) const
{
	return glGetUniformLocation(_shader_prog, name.c_str());
}

bool Shader::validate_shader_compile(GLuint shader)
{
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if constexpr (Logger::enabled())
	{
		if (!success)
		{
			GLint error_length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &error_length);

			std::vector<GLchar> error_log(error_length);
			glGetShaderInfoLog(shader, error_length, nullptr, error_log.data());
			Logger::get().log(LogSeverity::error, error_log.data());
		}
	}

	return success == GL_TRUE;
}

bool Shader::validate_shader_link(GLuint shader)
{
	GLint success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);

	if constexpr (Logger::enabled())
	{
		if (!success)
		{
			GLint error_length;
			glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &error_length);

			std::vector<GLchar> error_log(error_length);
			glGetProgramInfoLog(shader, error_length, nullptr, error_log.data());
			Logger::get().log(LogSeverity::error, error_log.data());
		}
	}

	return success == GL_TRUE;
}
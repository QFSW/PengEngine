#include "shader.h"

#include <cassert>

#include <memory/weak_ptr.h>
#include <core/logger.h>
#include <utils/utils.h>
#include <utils/io.h>

#include "primitives.h"

using namespace rendering;
using namespace math;

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
	_program = glCreateProgram();
	glAttachShader(_program, vert_shader);
	glAttachShader(_program, frag_shader);
	glLinkProgram(_program);
	_broken |= !validate_shader_link(_program);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	if (!_broken)
	{
		Logger::get().log(LogSeverity::log, "Extracting uniform information");

		GLint num_uniforms;
		glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &num_uniforms);

		_uniforms.resize(num_uniforms);
		for (GLint location = 0; location < num_uniforms; location++)
		{
			constexpr int32_t buf_size = 512;
			char name_buf[buf_size];
			GLint name_length;
			GLint size;
			GLenum type;

			glGetActiveUniform(_program, location, buf_size, &name_length, &size, &type, name_buf);

			Uniform& uniform = _uniforms[location];
			uniform.location = location;
			uniform.name = name_buf;
			uniform.type = type;
			uniform.default_value = read_uniform(uniform);
		}
	}
}

Shader::~Shader()
{
	Logger::get().logf(LogSeverity::log, "Destroying shader '%s'", _name.c_str());
	glDeleteProgram(_program);
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
	glUseProgram(_program);
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
	return glGetUniformLocation(_program, name.c_str());
}

std::vector<Shader::Uniform> Shader::uniforms() const
{
	return _uniforms;
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

std::optional<Shader::Parameter> Shader::read_uniform(const Uniform& uniform) const
{
	switch (uniform.type)
	{
		case GL_INT:
		{
			int32_t value;
			glGetnUniformiv(_program, uniform.location, sizeof(value), &value);

			return value;
		}
		case GL_UNSIGNED_INT:
		{
			uint32_t value;
			glGetnUniformuiv(_program, uniform.location, sizeof(value), &value);

			return value;
		}
		case GL_FLOAT:
		{
			float value;
			glGetnUniformfv(_program, uniform.location, sizeof(value), &value);

			return value;
		}
		case GL_DOUBLE:
		{
			double value;
			glGetnUniformdv(_program, uniform.location, sizeof(value), &value);

			return value;
		}
		case GL_INT_VEC2:
		{
			int32_t value[2];
			glGetnUniformiv(_program, uniform.location, sizeof(value), value);

			return Vector2i(value[0], value[1]);
		}
		case GL_UNSIGNED_INT_VEC2:
		{
			uint32_t value[2];
			glGetnUniformuiv(_program, uniform.location, sizeof(value), value);

			return Vector2u(value[0], value[1]);
		}
		case GL_FLOAT_VEC2:
		{
			float value[2];
			glGetnUniformfv(_program, uniform.location, sizeof(value), value);

			return Vector2f(value[0], value[1]);
		}
		case GL_DOUBLE_VEC2:
		{
			double value[2];
			glGetnUniformdv(_program, uniform.location, sizeof(value), value);

			return Vector2d(value[0], value[1]);
		}
		case GL_INT_VEC3:
		{
			int32_t value[3];
			glGetnUniformiv(_program, uniform.location, sizeof(value), value);

			return Vector3i(value[0], value[1], value[2]);
		}
		case GL_UNSIGNED_INT_VEC3:
		{
			uint32_t value[3];
			glGetnUniformuiv(_program, uniform.location, sizeof(value), value);

			return Vector3u(value[0], value[1], value[2]);
		}
		case GL_FLOAT_VEC3:
		{
			float value[3];
			glGetnUniformfv(_program, uniform.location, sizeof(value), value);

			return Vector3f(value[0], value[1], value[2]);
		}
		case GL_DOUBLE_VEC3:
		{
			double value[3];
			glGetnUniformdv(_program, uniform.location, sizeof(value), value);

			return Vector3d(value[0], value[1], value[2]);
		}
		case GL_INT_VEC4:
		{
			int32_t value[4];
			glGetnUniformiv(_program, uniform.location, sizeof(value), value);

			return Vector4i(value[0], value[1], value[2], value[3]);
		}
		case GL_UNSIGNED_INT_VEC4:
		{
			uint32_t value[4];
			glGetnUniformuiv(_program, uniform.location, sizeof(value), value);

			return Vector4u(value[0], value[1], value[2], value[3]);
		}
		case GL_FLOAT_VEC4:
		{
			float value[4];
			glGetnUniformfv(_program, uniform.location, sizeof(value), value);

			return Vector4f(value[0], value[1], value[2], value[3]);
		}
		case GL_DOUBLE_VEC4:
		{
			double value[4];
			glGetnUniformdv(_program, uniform.location, sizeof(value), value);

			return Vector4d(value[0], value[1], value[2], value[3]);
		}
		case GL_FLOAT_MAT4:
		{
			std::array<float, 16> value;
			glGetnUniformfv(_program, uniform.location, sizeof(value), value.data());

			return Matrix4x4f(value);
		}
		case GL_SAMPLER_2D:
		{
			return Primitives::white_tex();
		}
		default:
		{
			Logger::get().logf(LogSeverity::error,
				"Unable to get default value of parameter %s due to unsupported uniform type 0x%x",
				uniform.name.c_str(), uniform.type
			);

			return std::nullopt;
		}
	}
}

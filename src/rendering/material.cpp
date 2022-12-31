#include "material.h"

#include <core/logger.h>
#include <utils/functional.h>
#include <utils/utils.h>

using namespace rendering;
using namespace math;

Material::Material(peng::shared_ref<const Shader>&& shader)
	: _shader(std::move(shader))
	, _num_bound_textures(0)
{
	if (_shader->broken())
	{
		Logger::get().logf(LogSeverity::warning,
			"Provided shader '%s' is broken - switching to fallback",
			_shader->name().c_str()
		);

		_shader = Shader::fallback();
	}
}

Material::Material(const peng::shared_ref<const Shader>& shader)
	: Material(utils::copy(shader))
{ }

void Material::set_parameter(GLint uniform_location, const Parameter& parameter)
{
	if (const auto it = _existing_parameters.find(uniform_location); it != _existing_parameters.end())
	{
		std::get<Parameter>(_set_parameters[it->second]) = parameter;
		return;
	}

	_existing_parameters[uniform_location] = _set_parameters.size();
	_set_parameters.emplace_back(uniform_location, parameter);
}

void Material::set_parameter(const std::string& parameter_name, const Parameter& parameter)
{
	const GLint parameter_index = _shader->get_uniform_location(parameter_name);
	if (parameter_index >= 0)
	{
		set_parameter(parameter_index, parameter);
	}
	else if (_bad_parameter_names.find(parameter_name) == _bad_parameter_names.end())
	{
		_bad_parameter_names.insert(parameter_name);
		Logger::get().logf(LogSeverity::error,
			"Could not set parameter '%s' as no matching uniform could be found in the shader", parameter_name.c_str()
		);
	}
}

void Material::use()
{
	_num_bound_textures = 0;

	_shader->use();
	for (const auto& [location, parameter] : _set_parameters)
	{
		std::visit(functional::overload{
			[&](const auto& x) { apply_parameter(location, x); }
		}, parameter);
	}
}

peng::shared_ref<const Shader> Material::shader() const
{
	return _shader;
}

void Material::apply_parameter(GLint location, int32_t value)
{
	glUniform1i(location, value);
}

void Material::apply_parameter(GLint location, uint32_t value)
{
	glUniform1ui(location, value);
}

void Material::apply_parameter(GLint location, float value)
{
	glUniform1f(location, value);
}

void Material::apply_parameter(GLint location, double value)
{
	glUniform1d(location, value);
}

void Material::apply_parameter(GLint location, const Vector2i& value)
{
	glUniform2i(location, value.x, value.y);
}

void Material::apply_parameter(GLint location, const Vector2u& value)
{
	glUniform2ui(location, value.x, value.y);
}

void Material::apply_parameter(GLint location, const Vector2f& value)
{
	glUniform2f(location, value.x, value.y);
}

void Material::apply_parameter(GLint location, const Vector2d& value)
{
	glUniform2d(location, value.x, value.y);
}

void Material::apply_parameter(GLint location, const Vector3i& value)
{
	glUniform3i(location, value.x, value.y, value.z);
}

void Material::apply_parameter(GLint location, const Vector3u& value)
{
	glUniform3ui(location, value.x, value.y, value.z);
}

void Material::apply_parameter(GLint location, const Vector3f& value)
{
	glUniform3f(location, value.x, value.y, value.z);
}

void Material::apply_parameter(GLint location, const Vector3d& value)
{
	glUniform3d(location, value.x, value.y, value.z);
}

void Material::apply_parameter(GLint location, const Vector4i& value)
{
	glUniform4i(location, value.x, value.y, value.z, value.w);
}

void Material::apply_parameter(GLint location, const Vector4u& value)
{
	glUniform4ui(location, value.x, value.y, value.z, value.w);
}

void Material::apply_parameter(GLint location, const Vector4f& value)
{
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Material::apply_parameter(GLint location, const Vector4d& value)
{
	glUniform4d(location, value.x, value.y, value.z, value.w);
}

void Material::apply_parameter(GLint location, const Matrix4x4f& value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, value.elements.data());
}

void Material::apply_parameter(GLint location, const Matrix4x4d& value)
{
	glUniformMatrix4dv(location, 1, GL_FALSE, value.elements.data());
}

void Material::apply_parameter(GLint location, const peng::shared_ref<const Texture>& texture)
{
	if (_num_bound_textures >= 16)
	{
		throw std::runtime_error("Cannot bind more than 16 textures to a material");
	}

	const GLuint texture_slot = _num_bound_textures++;

	texture->use(GL_TEXTURE0 + texture_slot);
	glUniform1i(location, texture_slot);
}

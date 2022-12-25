#include "material.h"

#include <utils/functional.h>

using namespace rendering;
using namespace math;

Material::Material(const peng::shared_ref<Shader>& shader)
	: _shader(shader)
{ }

void Material::set_parameter(GLint uniform_location, const Parameter& parameter)
{
	if (const auto it = _existing_parameters.find(uniform_location); it != _existing_parameters.end())
	{
		std::get<Parameter>(_set_parameters[it->second]) = parameter;
	}

	_existing_parameters[uniform_location] = _set_parameters.size();
	_set_parameters.push_back(std::make_tuple(uniform_location, parameter));
}

void Material::set_parameter(const std::string& parameter_name, const Parameter& parameter)
{
	const GLint parameter_index = _shader->get_uniform_location(parameter_name);
	set_parameter(parameter_index, parameter);
}

void Material::use() const
{
	for (const auto& [location, parameter] : _set_parameters)
	{
		std::visit(functional::overload{
			[&](const auto& x) { set_uniform(location, x); }
		}, parameter);
	}

	_shader->use();
}

peng::shared_ref<Shader> Material::shader() const
{
	return _shader;
}

void Material::set_uniform(GLint location, float value) const
{
	glUniform1f(location, value);
}

void Material::set_uniform(GLint location, Vector2f value) const
{
	glUniform2f(location, value.x, value.y);
}

void Material::set_uniform(GLint location, Vector3f value) const
{
	glUniform3f(location, value.x, value.y, value.z);
}

void Material::set_uniform(GLint location, Vector4f value) const
{
	glUniform4f(location, value.x, value.y, value.z, value.w);
}
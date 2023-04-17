#include "material.h"

#include <core/logger.h>
#include <utils/functional.h>
#include <utils/utils.h>

#include "texture.h"
#include "texture_binding_cache.h"

using namespace rendering;
using namespace math;

Material::Material(peng::shared_ref<const Shader>&& shader)
    : _shader(std::move(shader))
    , _num_bound_textures(0)
{
    if (_shader->broken())
    {
        Logger::warning(
            "Provided shader '%s' is broken - switching to fallback",
            _shader->name().c_str()
        );

        _shader = Shader::fallback();
    }

    for (const Shader::Uniform& uniform : _shader->uniforms())
    {
        if (uniform.default_value)
        {
            set_parameter(uniform.location, *uniform.default_value);
        }
    }
}

Material::Material(const peng::shared_ref<const Shader>& shader)
    : Material(utils::copy(shader))
{ }

void Material::set_buffer(GLint buffer_index, const peng::shared_ref<const IShaderBuffer>& buffer)
{
    for (auto& [index, buf] : _bound_buffers)
    {
        if (index == buffer_index)
        {
            buf = buffer;
            return;
        }
    }

    _bound_buffers.emplace_back(buffer_index, buffer);
}

void Material::set_buffer(const std::string& buffer_name, const peng::shared_ref<const IShaderBuffer>& buffer)
{
    const GLint buffer_index = _shader->get_buffer_location(buffer_name);
    if (buffer_index >= 0)
    {
        set_buffer(buffer_index, buffer);
    }
    else if (!_bad_buffer_names.contains(buffer_name))
    {
        _bad_buffer_names.insert(buffer_name);
        Logger::error(
            "Could not set buffer '%s' as no matching buffer could be found in the shader '%s'",
            buffer_name.c_str(), _shader->name().c_str()
        );
    }
}

void Material::set_parameter(GLint uniform_location, const Shader::Parameter& parameter)
{
    if (const auto it = _existing_parameters.find(uniform_location); it != _existing_parameters.end())
    {
        std::get<Shader::Parameter>(_set_parameters[it->second]) = parameter;
        return;
    }

    _existing_parameters[uniform_location] = _set_parameters.size();
    _set_parameters.emplace_back(uniform_location, parameter);
}

void Material::set_parameter(const std::string& parameter_name, const Shader::Parameter& parameter)
{
    const GLint parameter_index = _shader->get_uniform_location(parameter_name);
    if (parameter_index >= 0)
    {
        set_parameter(parameter_index, parameter);
    }
    else if (!_bad_parameter_names.contains(parameter_name))
    {
        _bad_parameter_names.insert(parameter_name);
        Logger::error(
            "Could not set parameter '%s' as no matching uniform could be found in the shader '%s'",
            parameter_name.c_str(), _shader->name().c_str()
        );
    }
}

void Material::use()
{
    _shader->use();
    apply_uniforms();
    bind_buffers();
}

void Material::apply_uniforms()
{
    _num_bound_textures = 0;
    for (const auto& [location, parameter] : _set_parameters)
    {
        std::visit(functional::overload{
            [&](const auto& x) { apply_parameter(location, x); }
        }, parameter);
    }
}

void Material::bind_buffers()
{
    for (const auto& [index, buffer] : _bound_buffers)
    {
        _shader->bind_buffer(index, buffer);
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

void Material::apply_parameter(GLint location, const Matrix3x3f& value)
{
    glUniformMatrix3fv(location, 1, GL_FALSE, value.elements.data());
}

void Material::apply_parameter(GLint location, const Matrix3x3d& value)
{
    glUniformMatrix3dv(location, 1, GL_FALSE, value.elements.data());
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
    if (++_num_bound_textures >= 16)
    {
        throw std::runtime_error("Cannot bind more than 16 textures to a material");
    }

    const GLint texture_slot = TextureBindingCache::get().bind_texture(texture);
    glUniform1i(location, static_cast<GLint>(texture_slot));
}

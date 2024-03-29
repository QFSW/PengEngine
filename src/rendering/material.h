#pragma once

#include <tuple>
#include <unordered_map>
#include <unordered_set>

#include <memory/shared_ref.h>
#include <utils/concepts.h>

#include "shader.h"
#include "shader_buffer.h"

namespace rendering
{
    // TODO: turn into an Asset
    // TODO: refactor out UniformSet into its own object so that a draw call can be created
    //       without requiring that objects use unique copies of the material
    class Material
    {
    public:
        explicit Material(peng::shared_ref<const Shader>&& shader);
        explicit Material(const peng::shared_ref<const Shader>& shader);

        void use();
        void apply_uniforms();
        void bind_buffers();

        template <utils::variant_member<Shader::Parameter> T>
        void try_set_parameter(GLint uniform_location, const T& parameter)
        {
            if (uniform_location >= 0)
            {
                set_parameter(uniform_location, Shader::Parameter(parameter));
            }
        }

        template <utils::variant_member<Shader::Parameter> T>
        void set_parameter(GLint uniform_location, const T& parameter)
        {
            set_parameter(uniform_location, Shader::Parameter(parameter));
        }

        template <utils::variant_member<Shader::Parameter> T>
        void set_parameter(const std::string& parameter_name, const T& parameter)
        {
            set_parameter(parameter_name, Shader::Parameter(parameter));
        }

        // TODO: add a way to unset/unbind buffers
        void set_buffer(GLint buffer_index, const peng::shared_ref<const IShaderBuffer>& buffer);
        void set_buffer(const std::string& buffer_name, const peng::shared_ref<const IShaderBuffer>& buffer);

        [[nodiscard]] peng::shared_ref<const Shader> shader() const;

    private:
        void set_parameter(GLint uniform_location, const Shader::Parameter& parameter);
        void set_parameter(const std::string& parameter_name, const Shader::Parameter& parameter);

        void apply_parameter(GLint location, int32_t value);
        void apply_parameter(GLint location, uint32_t value);
        void apply_parameter(GLint location, float value);
        void apply_parameter(GLint location, double value);
        void apply_parameter(GLint location, const math::Vector2i& value);
        void apply_parameter(GLint location, const math::Vector2u& value);
        void apply_parameter(GLint location, const math::Vector2f& value);
        void apply_parameter(GLint location, const math::Vector2d& value);
        void apply_parameter(GLint location, const math::Vector3i& value);
        void apply_parameter(GLint location, const math::Vector3u& value);
        void apply_parameter(GLint location, const math::Vector3f& value);
        void apply_parameter(GLint location, const math::Vector3d& value);
        void apply_parameter(GLint location, const math::Vector4i& value);
        void apply_parameter(GLint location, const math::Vector4u& value);
        void apply_parameter(GLint location, const math::Vector4f& value);
        void apply_parameter(GLint location, const math::Vector4d& value);
        void apply_parameter(GLint location, const math::Matrix3x3f& value);
        void apply_parameter(GLint location, const math::Matrix3x3d& value);
        void apply_parameter(GLint location, const math::Matrix4x4f& value);
        void apply_parameter(GLint location, const math::Matrix4x4d& value);
        void apply_parameter(GLint location, const peng::shared_ref<const Texture>& texture);

        peng::shared_ref<const Shader> _shader;

        std::vector<std::tuple<GLint, Shader::Parameter>> _set_parameters;
        std::vector<std::tuple<GLint, peng::shared_ref<const IShaderBuffer>>> _bound_buffers;
        std::unordered_map<GLint, size_t> _existing_parameters;
        std::unordered_set<std::string> _bad_parameter_names;
        std::unordered_set<std::string> _bad_buffer_names;
        uint32_t _num_bound_textures;
    };
}

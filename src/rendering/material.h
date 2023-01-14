#pragma once

#include <tuple>

#include <common/common.h>
#include <memory/shared_ref.h>
#include <utils/concepts.h>

#include "shader.h"

namespace rendering
{
	class Material
	{
	public:
		explicit Material(peng::shared_ref<const Shader>&& shader);
		explicit Material(const peng::shared_ref<const Shader>& shader);

		void use();

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
		common::unordered_map<GLint, size_t> _existing_parameters;
		common::unordered_set<std::string> _bad_parameter_names;
		uint32_t _num_bound_textures;
	};
}

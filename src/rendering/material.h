#pragma once

#include <vector>
#include <variant>
#include <tuple>

#include <common/common.h>
#include <memory/shared_ref.h>
#include <math/matrix4x4.h>

#include "shader.h"
#include "texture.h"

namespace rendering
{
	class Material
	{
	public:
		using Parameter = std::variant<
			float,
			math::Vector2f,
			math::Vector3f,
			math::Vector4f,
			math::Matrix4x4f,
			peng::shared_ref<const Texture>
		>;

		explicit Material(const peng::shared_ref<const Shader>& shader);

		void set_parameter(GLint uniform_location, const Parameter& parameter);
		void set_parameter(const std::string& parameter_name, const Parameter& parameter);

		void use();
		peng::shared_ref<const Shader> shader() const;

	private:
		void apply_parameter(GLint location, float value);
		void apply_parameter(GLint location, const math::Vector2f& value);
		void apply_parameter(GLint location, const math::Vector3f& value);
		void apply_parameter(GLint location, const math::Vector4f& value);
		void apply_parameter(GLint location, const math::Matrix4x4f& value);
		void apply_parameter(GLint location, const peng::shared_ref<const Texture>& texture);

		peng::shared_ref<const Shader> _shader;

		std::vector<std::tuple<GLint, Parameter>> _set_parameters;
		common::unordered_map<GLint, size_t> _existing_parameters;
		uint32_t _num_bound_textures;
	};
}
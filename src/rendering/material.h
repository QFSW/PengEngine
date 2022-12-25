#pragma once

#include <vector>
#include <variant>
#include <tuple>

#include <common/common.h>
#include <memory/shared_ref.h>
#include <math/matrix4x4.h>

#include "shader.h"

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
			math::Matrix4x4f
		>;

		explicit Material(const peng::shared_ref<Shader>& shader);

		void set_parameter(GLint uniform_location, const Parameter& parameter);
		void set_parameter(const std::string& parameter_name, const Parameter& parameter);

		void use() const;
		peng::shared_ref<Shader> shader() const;

	private:
		void set_uniform(GLint location, float value) const;
		void set_uniform(GLint location, const math::Vector2f& value) const;
		void set_uniform(GLint location, const math::Vector3f& value) const;
		void set_uniform(GLint location, const math::Vector4f& value) const;
		void set_uniform(GLint location, const math::Matrix4x4f& value) const;

		peng::shared_ref<Shader> _shader;

		std::vector<std::tuple<GLint, Parameter>> _set_parameters;
		common::unordered_map<GLint, size_t> _existing_parameters;
	};
}
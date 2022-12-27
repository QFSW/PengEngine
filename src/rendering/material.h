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
			int32_t,
			uint32_t,
			float,
			double,
			math::Vector2i,
			math::Vector2u,
			math::Vector2f,
			math::Vector2d,
			math::Vector3i,
			math::Vector3u,
			math::Vector3f,
			math::Vector3d,
			math::Vector4i,
			math::Vector4u,
			math::Vector4f,
			math::Vector4d,
			math::Matrix4x4f,
			math::Matrix4x4d,
			peng::shared_ref<const Texture>
		>;

		explicit Material(const peng::shared_ref<const Shader>& shader);

		void set_parameter(GLint uniform_location, const Parameter& parameter);
		void set_parameter(const std::string& parameter_name, const Parameter& parameter);

		void use();

		[[nodiscard]] peng::shared_ref<const Shader> shader() const;

	private:
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
		void apply_parameter(GLint location, const math::Matrix4x4f& value);
		void apply_parameter(GLint location, const math::Matrix4x4d& value);
		void apply_parameter(GLint location, const peng::shared_ref<const Texture>& texture);

		peng::shared_ref<const Shader> _shader;

		std::vector<std::tuple<GLint, Parameter>> _set_parameters;
		common::unordered_map<GLint, size_t> _existing_parameters;
		uint32_t _num_bound_textures;
	};
}
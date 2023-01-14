#pragma once

#include <string>
#include <vector>
#include <variant>
#include <optional>

#include <GL/glew.h>
#include <memory/shared_ref.h>
#include <math/matrix3x3.h>
#include <math/matrix4x4.h>

#include "texture.h"

namespace rendering
{
	class Shader
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
			math::Matrix3x3f,
			math::Matrix3x3d,
			math::Matrix4x4f,
			math::Matrix4x4d,
			peng::shared_ref<const Texture>
		>;

		struct Uniform
		{
			GLint location = -1;
			std::string name;
			GLenum type = GL_INT;
			std::optional<Parameter> default_value;
		};

		Shader(
			const std::string& name,
			const std::string& vert_shader_path, 
			const std::string& frag_shader_path
		);

		Shader(
			std::string&& name,
			const std::string& vert_shader_path,
			const std::string& frag_shader_path
		);

		Shader(const Shader&) = delete;
		Shader(Shader&&) = delete;
		~Shader();

		static peng::shared_ref<const Shader> fallback();

		void use() const;

		[[nodiscard]] const std::string& name() const noexcept;
		[[nodiscard]] bool broken() const noexcept;

		[[nodiscard]] GLint get_uniform_location(const std::string& name) const;
		[[nodiscard]] std::vector<Uniform> uniforms() const;

	private:
		bool validate_shader_compile(GLuint shader) const;
		bool validate_shader_link(GLuint shader) const;

		[[nodiscard]] std::optional<Parameter> read_uniform(const Uniform& uniform) const;

		std::string _name;
		GLuint _program;
		bool _broken;

		std::vector<Uniform> _uniforms;
	};
}
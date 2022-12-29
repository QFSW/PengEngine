#pragma once

#include <string>

#include <GL/glew.h>
#include <memory/shared_ref.h>

namespace rendering
{
	class Shader
	{
	public:
		Shader(const std::string& vert_shader_path, const std::string& frag_shader_path);
		Shader(const Shader&) = delete;
		Shader(Shader&&) = delete;
		~Shader();

		static peng::shared_ref<const Shader> fallback();

		void use() const;

		[[nodiscard]] bool broken() const noexcept;
		[[nodiscard]] GLint get_uniform_location(const std::string& name) const;

	private:
		bool validate_shader_compile(GLuint shader);
		bool validate_shader_link(GLuint shader);

		bool _broken;
		GLuint _shader_prog;
	};
}
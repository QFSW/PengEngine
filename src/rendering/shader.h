#pragma once

#include <string>

#include <GL/glew.h>
#include <memory/shared_ref.h>

namespace rendering
{
	class Shader
	{
	public:
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

	private:
		bool validate_shader_compile(GLuint shader);
		bool validate_shader_link(GLuint shader);

		std::string _name;
		GLuint _shader_prog;
		bool _broken;
	};
}
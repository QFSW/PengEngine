#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

#include "shader_type.h"
#include "shader_symbol.h"

namespace rendering
{
	struct PreprocessedShader
	{
		ShaderType type;
		std::string contents;
		std::vector<ShaderSymbol> symbols;
	};

	class ShaderCompiler
	{
	public:
		[[nodiscard]] PreprocessedShader preprocess_shader(const std::string& path, ShaderType type) const;
		[[nodiscard]] PreprocessedShader preprocess_shader(const std::string& path, ShaderType type, const std::string& src) const;
		[[nodiscard]] GLuint compile_shader(const PreprocessedShader& preprocessed_shader) const;

		void add_include_path(const std::string& include_path);

	private:
		std::vector<std::string> _include_roots;
	};
}

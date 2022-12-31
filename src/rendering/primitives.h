#pragma once

#include <memory/weak_ptr.h>

namespace rendering
{
	class Mesh;
	class Texture;
	class Shader;
	class Material;

	class Primitives
	{
	public:
		[[nodiscard]] static peng::shared_ref<const Mesh> cube();
		[[nodiscard]] static peng::shared_ref<const Mesh> fullscreen_quad();

		[[nodiscard]] static peng::shared_ref<const Texture> white_tex();

		[[nodiscard]] static peng::shared_ref<const Shader> unlit_shader();

		[[nodiscard]] static peng::shared_ref<const Material> unlit_material();
	};
}
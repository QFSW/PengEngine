#pragma once

#include <memory/weak_ptr.h>

#include "mesh.h"
#include "texture.h"

namespace rendering
{
	class Primitives
	{
	public:
		[[nodiscard]] static peng::shared_ref<const Mesh> cube();
		[[nodiscard]] static peng::shared_ref<const Mesh> fullscreen_quad();

		[[nodiscard]] static peng::shared_ref<const Texture> white_tex();

	private:
		static peng::weak_ptr<const Mesh> _cube;
		static peng::weak_ptr<const Mesh> _fullscreen_quad;

		static peng::weak_ptr<const Texture> _white_tex;
	};
}
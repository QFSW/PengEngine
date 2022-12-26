#pragma once

#include <memory/weak_ptr.h>

#include "mesh.h"

namespace rendering
{
	class Primitives
	{
	public:
		[[nodiscard]] static peng::shared_ref<const Mesh> cube();
		[[nodiscard]] static peng::shared_ref<const Mesh> fullscreen_quad();

	private:
		static peng::weak_ptr<const Mesh> _cube;
		static peng::weak_ptr<const Mesh> _fullscreen_quad;
	};
}
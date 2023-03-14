#pragma once

#include <memory/shared_ref.h>

namespace rendering
{
	class Mesh;
	class Material;

	struct DrawCall
	{
		peng::shared_ref<const Mesh> mesh;
		peng::shared_ref<Material> material;
	};
}

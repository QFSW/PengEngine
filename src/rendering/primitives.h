#pragma once

#include <rendering/vertex.h>
#include <memory/weak_ptr.h>
#include <math/vector3.h>

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
		[[nodiscard]] static peng::shared_ref<const Mesh> cube_uv();
		[[nodiscard]] static peng::shared_ref<const Mesh> fullscreen_quad();
		[[nodiscard]] static peng::shared_ref<const Mesh> icosahedron();
		[[nodiscard]] static peng::shared_ref<const Mesh> icosphere(uint32_t order = 3);

		[[nodiscard]] static peng::shared_ref<const Texture> white_tex();

		[[nodiscard]] static peng::shared_ref<const Shader> unlit_shader();
		[[nodiscard]] static peng::shared_ref<const Shader> phong_shader();
		[[nodiscard]] static peng::shared_ref<const Shader> skybox_shader();

		[[nodiscard]] static peng::shared_ref<Material> unlit_material();
		[[nodiscard]] static peng::shared_ref<Material> phong_material();
		[[nodiscard]] static peng::shared_ref<Material> skybox_material();

	private:
		static std::vector<Vertex> icosahedron_vertices(float radius);
		static std::vector<math::Vector3u> icosahedron_indices();
	};
}

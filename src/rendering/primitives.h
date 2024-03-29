#pragma once

#include <rendering/vertex.h>
#include <memory/weak_ptr.h>
#include <math/vector3.h>

namespace rendering
{
    class Mesh;
    class Texture;
    class Sprite;
    class Shader;
    class Material;
    class BitmapFont;

    class Primitives
    {
    public:
        [[nodiscard]] static peng::shared_ref<const Mesh> cube();
        [[nodiscard]] static peng::shared_ref<const Mesh> cube_uv();
        [[nodiscard]] static peng::shared_ref<const Mesh> quad();
        [[nodiscard]] static peng::shared_ref<const Mesh> fullscreen_quad();
        [[nodiscard]] static peng::shared_ref<const Mesh> icosahedron();
        [[nodiscard]] static peng::shared_ref<const Mesh> icosphere(uint32_t order = 3);

        [[nodiscard]] static peng::shared_ref<const Texture> white_tex();

        [[nodiscard]] static peng::shared_ref<const Sprite> white_sprite();

        [[nodiscard]] static peng::shared_ref<const Shader> unlit_shader();
        [[nodiscard]] static peng::shared_ref<const Shader> unlit_alpha_shader();
        [[nodiscard]] static peng::shared_ref<const Shader> sprite_shader();
        [[nodiscard]] static peng::shared_ref<const Shader> sprite_alpha_shader();
        [[nodiscard]] static peng::shared_ref<const Shader> sprite_instanced_shader();
        [[nodiscard]] static peng::shared_ref<const Shader> sprite_instanced_alpha_shader();
        [[nodiscard]] static peng::shared_ref<const Shader> phong_shader();
        [[nodiscard]] static peng::shared_ref<const Shader> skybox_shader();

        [[nodiscard]] static peng::shared_ref<Material> unlit_material();
        [[nodiscard]] static peng::shared_ref<Material> unlit_alpha_material();
        [[nodiscard]] static peng::shared_ref<Material> phong_material();
        [[nodiscard]] static peng::shared_ref<Material> skybox_material();

        [[nodiscard]] static peng::shared_ref<const BitmapFont> peng_font();

    private:
        static std::vector<Vertex> icosahedron_vertices(float radius);
        static std::vector<math::Vector3u> icosahedron_indices();
    };
}

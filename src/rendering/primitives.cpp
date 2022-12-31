#include "primitives.h"

#include <memory/shared_ref.h>
#include <math/vector3.h>

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "material.h"

using namespace rendering;
using namespace math;

peng::shared_ref<const Mesh> Primitives::cube()
{
    static peng::weak_ptr<const Mesh> weak_cube;
	if (const peng::shared_ptr<const Mesh> strong_cube = weak_cube.lock())
	{
		return strong_cube.to_shared_ref();
	}

    const std::vector<Vector3f> vertices =
    {
        Vector3f(-0.5f, -0.5f, -0.5f),
        Vector3f(0.5f, -0.5f, -0.5f),
        Vector3f(0.5f, 0.5f, -0.5f),
        Vector3f(-0.5f, 0.5f, -0.5f),

        Vector3f(-0.5f, -0.5f, 0.5f),
        Vector3f(0.5f, -0.5f, 0.5f),
        Vector3f(0.5f, 0.5f, 0.5f),
        Vector3f(-0.5f, 0.5f, 0.5f),

        Vector3f(-0.5f, -0.5f, -0.5f),
        Vector3f(-0.5f, 0.5f, -0.5f),
        Vector3f(-0.5f, 0.5f, 0.5f),
        Vector3f(-0.5f, -0.5f, 0.5f),

        Vector3f(0.5f, -0.5f, -0.5f),
        Vector3f(0.5f, 0.5f, -0.5f),
        Vector3f(0.5f, 0.5f, 0.5f),
        Vector3f(0.5f, -0.5f, 0.5f),

        Vector3f(-0.5f, 0.5f, -0.5f),
        Vector3f(0.5f, 0.5f, -0.5f),
        Vector3f(0.5f, 0.5f, 0.5f),
        Vector3f(-0.5f, 0.5f, 0.5f),

        Vector3f(-0.5f, -0.5f, -0.5f),
        Vector3f(0.5f, -0.5f, -0.5f),
        Vector3f(0.5f, -0.5f, 0.5f),
        Vector3f(-0.5f, -0.5f, 0.5f),
    };

    const std::vector<Vector3u> indices =
    {
        Vector3u(0, 1, 3),
        Vector3u(3, 1, 2),

        Vector3u(4, 5, 7),
        Vector3u(7, 5, 6),

        Vector3u(8, 9, 11),
        Vector3u(11, 9, 10),

        Vector3u(12, 13, 15),
        Vector3u(15, 13, 14),

        Vector3u(16, 17, 19),
        Vector3u(19, 17, 18),

        Vector3u(20, 21, 23),
        Vector3u(23, 21, 22),
    };

    const std::vector<Vector2f> tex_coords =
    {
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1),
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1),
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1),
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1),
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1),
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1),
    };

	peng::shared_ref<Mesh> cube = peng::make_shared<Mesh>(
        "Cube", vertices, indices, std::vector<Vector3f>(), tex_coords
    );

    weak_cube = cube;
    return cube;
}

peng::shared_ref<const Mesh> Primitives::fullscreen_quad()
{
    static peng::weak_ptr<const Mesh> weak_quad;
    if (const peng::shared_ptr<const Mesh> strong_quad = weak_quad.lock())
    {
        return strong_quad.to_shared_ref();
    }

    const std::vector<Vector3f> vertices =
    {
        Vector3f(-1, -1, 0),
        Vector3f(1, -1, 0),
        Vector3f(1, 1, 0),
        Vector3f(-1, 1, 0),
    };

    const std::vector<Vector3u> indices =
    {
        Vector3u(0, 1, 3),
        Vector3u(3, 1, 2),
    };

    const std::vector<Vector2f> tex_coords =
    {
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1),
    };

    peng::shared_ref<Mesh> quad = peng::make_shared<Mesh>(
        "Fullscreen Quad", vertices, indices, std::vector<Vector3f>(), tex_coords
    );

    weak_quad = quad;
    return quad;
}

peng::shared_ref<const Texture> Primitives::white_tex()
{
    static peng::weak_ptr<const Texture> weak_tex;
    if (const peng::shared_ptr<const Texture> strong_tex = weak_tex.lock())
    {
        return strong_tex.to_shared_ref();
    }

    const std::vector<Vector3u8> rgb_data = { Vector3u8(0xFF, 0xFF, 0xFF) };
    peng::shared_ref<Texture> white_tex = peng::make_shared<Texture>(
        "White 1x1px", rgb_data, Vector2i::one()
    );

    weak_tex = white_tex;
    return white_tex;
}

peng::shared_ref<const Shader> Primitives::unlit_shader()
{
    static peng::weak_ptr<const Shader> weak_shader;
    if (const peng::shared_ptr<const Shader> strong_shader = weak_shader.lock())
    {
        return strong_shader.to_shared_ref();
    }

    peng::shared_ref<Shader> unlit = peng::make_shared<Shader>(
        "Unlit",
        "resources/shaders/core/projection.vert",
        "resources/shaders/core/unlit.frag"
    );

    weak_shader = unlit;
    return unlit;
}

peng::shared_ref<const Material> Primitives::unlit_material()
{
    static peng::weak_ptr<const Material> weak_material;
    if (const peng::shared_ptr<const Material> strong_material = weak_material.lock())
    {
        return strong_material.to_shared_ref();
    }

    peng::shared_ref<Material> unlit = peng::make_shared<Material>(
        unlit_shader()
    );

    unlit->set_parameter("tex_scale", Vector2f::one());
    unlit->set_parameter("base_color", Vector4f(1, 1, 1, 1));
    unlit->set_parameter("color_tex", white_tex());

    weak_material = unlit;
    return unlit;
}

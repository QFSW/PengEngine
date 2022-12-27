#include "primitives.h"

#include <memory/shared_ref.h>
#include <math/vector3.h>

using namespace rendering;
using namespace math;

peng::weak_ptr<const Mesh> Primitives::_cube;
peng::weak_ptr<const Mesh> Primitives::_fullscreen_quad;

peng::weak_ptr<const Texture> Primitives::_white_tex;

peng::shared_ref<const Mesh> Primitives::cube()
{
	if (const peng::shared_ptr<const Mesh> strong_cube = _cube.lock())
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

    _cube = cube;
    return cube;
}

peng::shared_ref<const Mesh> Primitives::fullscreen_quad()
{
    if (const peng::shared_ptr<const Mesh> strong_quad = _fullscreen_quad.lock())
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

    _fullscreen_quad = quad;
    return quad;
}

peng::shared_ref<const Texture> Primitives::white_tex()
{
    if (const peng::shared_ptr<const Texture> strong_tex = _white_tex.lock())
    {
        return strong_tex.to_shared_ref();
    }

    const std::vector<Vector3u8> rgb_data = { Vector3u8(0xFF, 0xFF, 0xFF)};
    peng::shared_ref<Texture> white_tex = peng::make_shared<Texture>(
        "White 1x1px", rgb_data, Vector2i::one()
    );

    _white_tex = white_tex;
    return white_tex;
}

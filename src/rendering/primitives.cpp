#include "primitives.h"

#include <memory/shared_ref.h>
#include <math/vector3.h>
#include <rendering/utils.h>
#include <utils/strtools.h>

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

    const std::vector<Vertex> vertices =
    {
        Vertex(Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(0, 0, -1), Vector2f(0, 0)),
        Vertex(Vector3f(0.5f, -0.5f, -0.5f), Vector3f(0, 0, -1), Vector2f(1, 0)),
        Vertex(Vector3f(0.5f, 0.5f, -0.5f), Vector3f(0, 0, -1), Vector2f(1, 1)),
        Vertex(Vector3f(-0.5f, 0.5f, -0.5f), Vector3f(0, 0, -1), Vector2f(0, 1)),

        Vertex(Vector3f(-0.5f, -0.5f, 0.5f), Vector3f(0, 0, 1), Vector2f(0, 0)),
        Vertex(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(0, 0, 1), Vector2f(1, 0)),
        Vertex(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(0, 0, 1), Vector2f(1, 1)),
        Vertex(Vector3f(-0.5f, 0.5f, 0.5f), Vector3f(0, 0, 1), Vector2f(0, 1)),

        Vertex(Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(-1, 0, 0), Vector2f(0, 0)),
        Vertex(Vector3f(-0.5f, 0.5f, -0.5f), Vector3f(-1, 0, 0), Vector2f(1, 0)),
        Vertex(Vector3f(-0.5f, 0.5f, 0.5f), Vector3f(-1, 0, 0), Vector2f(1, 1)),
        Vertex(Vector3f(-0.5f, -0.5f, 0.5f), Vector3f(-1, 0, 0), Vector2f(0, 1)),

        Vertex(Vector3f(0.5f, -0.5f, -0.5f), Vector3f(1, 0, 0), Vector2f(0, 0)),
        Vertex(Vector3f(0.5f, 0.5f, -0.5f), Vector3f(1, 0, 0), Vector2f(1, 0)),
        Vertex(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(1, 0, 0), Vector2f(1, 1)),
        Vertex(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(1, 0, 0), Vector2f(0, 1)),

        Vertex(Vector3f(-0.5f, 0.5f, -0.5f), Vector3f(0, 1, 0), Vector2f(0, 0)),
        Vertex(Vector3f(0.5f, 0.5f, -0.5f), Vector3f(0, 1, 0), Vector2f(1, 0)),
        Vertex(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(0, 1, 0), Vector2f(1, 1)),
        Vertex(Vector3f(-0.5f, 0.5f, 0.5f), Vector3f(0, 1, 0), Vector2f(0, 1)),

        Vertex(Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(0, -1, 0), Vector2f(0, 0)),
        Vertex(Vector3f(0.5f, -0.5f, -0.5f), Vector3f(0, -1, 0), Vector2f(1, 0)),
        Vertex(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(0, -1, 0), Vector2f(1, 1)),
        Vertex(Vector3f(-0.5f, -0.5f, 0.5f), Vector3f(0, -1, 0), Vector2f(0, 1)),
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

	peng::shared_ref<Mesh> cube = peng::make_shared<Mesh>(
        "Cube", vertices, indices
    );

    weak_cube = cube;
    return cube;
}

peng::shared_ref<const Mesh> Primitives::cube_uv()
{
    static peng::weak_ptr<const Mesh> weak_cube;
    if (const peng::shared_ptr<const Mesh> strong_cube = weak_cube.lock())
    {
        return strong_cube.to_shared_ref();
    }

    const std::vector<Vertex> vertices =
    {
        Vertex(Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(0, 0, -1), Vector2f(1, 1) / Vector2f(4, 3)),
        Vertex(Vector3f(0.5f, -0.5f, -0.5f), Vector3f(0, 0, -1), Vector2f(0, 1) / Vector2f(4, 3)),
        Vertex(Vector3f(0.5f, 0.5f, -0.5f), Vector3f(0, 0, -1), Vector2f(0, 2) / Vector2f(4, 3)),
        Vertex(Vector3f(-0.5f, 0.5f, -0.5f), Vector3f(0, 0, -1), Vector2f(1, 2) / Vector2f(4, 3)),

        Vertex(Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(-1, 0, 0), Vector2f(1, 1) / Vector2f(4, 3)),
        Vertex(Vector3f(-0.5f, -0.5f, 0.5f), Vector3f(-1, 0, 0), Vector2f(2, 1) / Vector2f(4, 3)),
        Vertex(Vector3f(-0.5f, 0.5f, 0.5f), Vector3f(-1, 0, 0), Vector2f(2, 2) / Vector2f(4, 3)),
        Vertex(Vector3f(-0.5f, 0.5f, -0.5f), Vector3f(-1, 0, 0), Vector2f(1, 2) / Vector2f(4, 3)),

        Vertex(Vector3f(-0.5f, -0.5f, 0.5f), Vector3f(0, 0, 1), Vector2f(2, 1) / Vector2f(4, 3)),
        Vertex(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(0, 0, 1), Vector2f(3, 1) / Vector2f(4, 3)),
        Vertex(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(0, 0, 1), Vector2f(3, 2) / Vector2f(4, 3)),
        Vertex(Vector3f(-0.5f, 0.5f, 0.5f), Vector3f(0, 0, 1), Vector2f(2, 2) / Vector2f(4, 3)),

        Vertex(Vector3f(0.5f, -0.5f, -0.5f), Vector3f(1, 0, 0), Vector2f(4, 1) / Vector2f(4, 3)),
        Vertex(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(1, 0, 0), Vector2f(3, 1) / Vector2f(4, 3)),
        Vertex(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(1, 0, 0), Vector2f(3, 2) / Vector2f(4, 3)),
        Vertex(Vector3f(0.5f, 0.5f, -0.5f), Vector3f(1, 0, 0), Vector2f(4, 2) / Vector2f(4, 3)),

        Vertex(Vector3f(-0.5f, 0.5f, -0.5f), Vector3f(0, 1, 0), Vector2f(1, 2) / Vector2f(4, 3)),
        Vertex(Vector3f(-0.5f, 0.5f, 0.5f), Vector3f(0, 1, 0), Vector2f(2, 2) / Vector2f(4, 3)),
        Vertex(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(0, 1, 0), Vector2f(2, 3) / Vector2f(4, 3)),
        Vertex(Vector3f(0.5f, 0.5f, -0.5f), Vector3f(0, 1, 0), Vector2f(1, 3) / Vector2f(4, 3)),

        Vertex(Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(0, -1, 0), Vector2f(1, 0) / Vector2f(4, 3)),
        Vertex(Vector3f(0.5f, -0.5f, -0.5f), Vector3f(0, -1, 0), Vector2f(2, 0) / Vector2f(4, 3)),
        Vertex(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(0, -1, 0), Vector2f(2, 1) / Vector2f(4, 3)),
        Vertex(Vector3f(-0.5f, -0.5f, 0.5f), Vector3f(0, -1, 0), Vector2f(1, 1) / Vector2f(4, 3)),
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

    peng::shared_ref<Mesh> cube = peng::make_shared<Mesh>(
        "Cube UV", vertices, indices
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

    const std::vector<Vertex> vertices =
    {
        Vertex(Vector3f(-1, -1, 0), Vector3f(0, 0, 1), Vector2f(0, 0)),
        Vertex(Vector3f(1, -1, 0), Vector3f(0, 0, 1), Vector2f(1, 0)),
        Vertex(Vector3f(1, 1, 0), Vector3f(0, 0, 1), Vector2f(1, 1)),
        Vertex(Vector3f(-1, 1, 0), Vector3f(0, 0, 1), Vector2f(0, 1)),
    };

    const std::vector<Vector3u> indices =
    {
        Vector3u(0, 1, 3),
        Vector3u(3, 1, 2),
    };

    peng::shared_ref<Mesh> quad = peng::make_shared<Mesh>(
        "Fullscreen Quad", vertices, indices
    );

    weak_quad = quad;
    return quad;
}

std::vector<Vertex> Primitives::icosahedron_vertices(float radius)
{
    const float t = (1 + std::sqrt(5.0f)) / 2;
    const float r = radius;
    const float x = r / t;

	std::vector<Vertex> vertices = {
        Vertex(Vector3f(-x, r, 0), Vector3f::zero(), Vector2f::zero()),
        Vertex(Vector3f(x, r, 0), Vector3f::zero(), Vector2f::zero()),
        Vertex(Vector3f(-x, -r, 0), Vector3f::zero(), Vector2f::zero()),
        Vertex(Vector3f(x, -r, 0), Vector3f::zero(), Vector2f::zero()),
        Vertex(Vector3f(0, -x, r), Vector3f::zero(), Vector2f::zero()),
        Vertex(Vector3f(0, x, r), Vector3f::zero(), Vector2f::zero()),
        Vertex(Vector3f(0, -x, -r), Vector3f::zero(), Vector2f::zero()),
        Vertex(Vector3f(0, x, -r), Vector3f::zero(), Vector2f::zero()),
        Vertex(Vector3f(r, 0, -x), Vector3f::zero(), Vector2f::zero()),
        Vertex(Vector3f(r, 0, x), Vector3f::zero(), Vector2f::zero()),
        Vertex(Vector3f(-r, 0, -x), Vector3f::zero(), Vector2f::zero()),
        Vertex(Vector3f(-r, 0, x), Vector3f::zero(), Vector2f::zero()),
    };

    return vertices;
}

std::vector<Vector3u> Primitives::icosahedron_indices()
{
    return std::vector<Vector3u>
	{
        Vector3u(0, 11, 5),
        Vector3u(0, 5, 1),
        Vector3u(0, 1, 7),
        Vector3u(0, 7, 10),
        Vector3u(0, 10, 11),
        Vector3u(1, 5, 9),
        Vector3u(5, 11, 4),
        Vector3u(11, 10, 2),
        Vector3u(10, 7, 6),
        Vector3u(7, 1, 8),
        Vector3u(3, 9, 4),
        Vector3u(3, 4, 2),
        Vector3u(3, 2, 6),
        Vector3u(3, 6, 8),
        Vector3u(3, 8, 9),
        Vector3u(4, 9, 5),
        Vector3u(2, 4, 11),
        Vector3u(6, 2, 10),
        Vector3u(8, 6, 7),
        Vector3u(9, 8, 1),
    };
}

peng::shared_ref<const Mesh> Primitives::icosahedron()
{
    return icosphere(0);
}

peng::shared_ref<const Mesh> Primitives::icosphere(uint32_t order)
{
    static std::unordered_map<uint32_t, peng::weak_ptr<const Mesh>> weak_icospheres;
    if (const auto it = weak_icospheres.find(order); it != weak_icospheres.end())
    {
        if (const peng::shared_ptr<const Mesh> strong_icosphere = it->second.lock())
        {
            return strong_icosphere.to_shared_ref();
        }
    }

    constexpr float radius = 0.5f;
    std::vector<Vertex> vertices = icosahedron_vertices(radius);
    std::vector<Vector3u> indices = icosahedron_indices();

    for (uint32_t level = 0; level < order; level++)
    {
        auto [vertices_s, indices_s] = subdivide(vertices, indices);
        vertices = std::move(vertices_s);
        indices = std::move(indices_s);
    }

    for (Vertex& vertex : vertices)
    {
        const Vector3f dir = vertex.position.normalized_unsafe();
        constexpr float inv_tau = std::numbers::inv_pi_v<float> / 2;

        vertex.position = dir * radius;
        vertex.normal = dir;
        vertex.tex_coord = Vector2f(
            std::atan2(dir.z, dir.x) * inv_tau,
            std::atan2(dir.y, dir.x) * inv_tau
        );
    }

    // TODO: add a mesh seam to fix weird texture coordinates

    peng::shared_ref<Mesh> icosphere = peng::make_shared<Mesh>(
        strtools::catf("Icosphere(%d)", order), vertices, indices
    );

    weak_icospheres[order] = icosphere;
    return icosphere;
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
    // TODO: work out a way to de-duplicate all this caching boilerplate
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

peng::shared_ref<const Shader> Primitives::phong_shader()
{
	static peng::weak_ptr<const Shader> weak_shader;
	if (const peng::shared_ptr<const Shader> strong_shader = weak_shader.lock())
	{
		return strong_shader.to_shared_ref();
	}
    \
	peng::shared_ref<Shader> phong = peng::make_shared<Shader>(
		"Phong",
		"resources/shaders/core/projection.vert",
		"resources/shaders/core/phong.frag"
	);

	weak_shader = phong;
	return phong;
}

peng::shared_ref<const Shader> Primitives::skybox_shader()
{
    static peng::weak_ptr<const Shader> weak_shader;
    if (const peng::shared_ptr<const Shader> strong_shader = weak_shader.lock())
    {
        return strong_shader.to_shared_ref();
    }

    peng::shared_ref<Shader> skybox = peng::make_shared<Shader>(
        "Skybox",
        "resources/shaders/core/skybox.vert",
        "resources/shaders/core/unlit.frag"
    );
#
    // Needs to be drawn after most other entities
    skybox->draw_order() = 100;

    weak_shader = skybox;
    return skybox;
}

peng::shared_ref<Material> Primitives::unlit_material()
{
    return peng::make_shared<Material>(
        unlit_shader()
    );
}

peng::shared_ref<Material> Primitives::phong_material()
{
	return peng::make_shared<Material>(
		phong_shader()
	);
}

peng::shared_ref<Material> Primitives::skybox_material()
{
    return peng::make_shared<Material>(
        skybox_shader()
    );
}

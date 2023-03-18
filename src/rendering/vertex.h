#pragma once

#include <math/vector3.h>

namespace rendering
{
    struct Vertex
    {
        math::Vector3f position;
        math::Vector3f normal;
        math::Vector2f tex_coord;
        math::Vector3f color;

        Vertex(const math::Vector3f& position, const math::Vector3f& normal, const math::Vector2f& tex_coord);
        Vertex(const math::Vector3f& position, const math::Vector3f& normal, const math::Vector2f& tex_coord, const math::Vector3f& color);

        [[nodiscard]] static Vertex subdivide(const Vertex& a, const Vertex& b);
    };
}

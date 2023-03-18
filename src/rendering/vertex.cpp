#include "vertex.h"

using namespace rendering;
using namespace math;

Vertex::Vertex(const Vector3f& position, const math::Vector3f& normal, const Vector2f& tex_coord)
    : position(position)
    , normal(normal)
    , tex_coord(tex_coord)
{ }

Vertex::Vertex(const Vector3f& position, const math::Vector3f& normal, const Vector2f& tex_coord, const Vector3f& color)
    : position(position)
    , normal(normal)
    , tex_coord(tex_coord)
    , color(color)
{ }

Vertex Vertex::subdivide(const Vertex& a, const Vertex& b)
{
    return Vertex(
        (a.position + b.position) / 2,
        (a.normal + b.normal) / 2,
        (a.tex_coord + b.tex_coord) / 2,
        (a.color + b.color) / 2
    );
}

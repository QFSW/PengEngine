#include "plane.h"

using namespace math;

Plane::Plane()
    : normal(0, 1, 0)
    , distance(0)
{ }

Plane::Plane(const Vector3f& normal, float distance)
    : normal(normal)
    , distance(distance)
{ }

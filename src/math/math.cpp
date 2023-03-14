#include "math.h"

#include <cmath>

float math::randf()
{
	return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

math::Vector2f math::rand2f()
{
	return Vector2f(randf(), randf());
}

math::Vector3f math::rand3f()
{
	return Vector3f(randf(), randf(), randf());
}

math::Vector4f math::rand4f()
{
	return Vector4f(randf(), randf(), randf(), randf());
}

float math::rand_range(float min, float max)
{
	return min + randf() * (max - min);
}

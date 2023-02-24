#include "math.h"

#include <cmath>

float math::randf()
{
	return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

float math::rand_range(float min, float max)
{
	return min + randf() * (max - min);
}

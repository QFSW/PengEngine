#pragma once

#include <vector>
#include <tuple>

#include "vertex.h"

namespace rendering
{
	[[nodiscard]] std::tuple<std::vector<Vertex>, std::vector<math::Vector3u>> subdivide(
		const std::vector<Vertex>& vertices,
		const std::vector<math::Vector3u>& indices
	);
}

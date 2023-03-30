#pragma once

#include <vector>

#include <memory/shared_ref.h>
#include <math/vector2.h>

namespace rendering
{
    class Sprite;
    class Texture;

    class SpriteSheet
    {
    public:
        // TODO: support padding
        [[nodiscard]] static std::vector<peng::shared_ref<const Sprite>> slice_grid(
            const peng::shared_ref<const Texture>& texture,
            float px_per_unit,
            const math::Vector2i& cell_size,
            int32_t cell_count
        );
    };
}
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
        struct Config
        {
            float px_per_unit;
            math::Vector2i cell_size;
            math::Vector2i cell_padding;
            int32_t cell_count;
        };

        [[nodiscard]] static std::vector<peng::shared_ref<const Sprite>> slice_grid(
            const peng::shared_ref<const Texture>& texture, const Config& config
        );

    private:
        [[nodiscard]] static int get_cell_count_1d(int row_length, int cell_length, int cell_padding);
    };
}
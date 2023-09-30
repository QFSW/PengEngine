#pragma once

#include <vector>

#include <memory/shared_ref.h>
#include <math/vector2.h>

class Archive;

namespace rendering
{
    class Sprite;
    class Texture;

    class SpriteSheet
    {
    public:
        struct Config
        {
            const peng::shared_ref<const Texture>& texture;
            float px_per_unit;
            math::Vector2i cell_size;
            math::Vector2i cell_padding;
            int32_t cell_count;
        };

        SpriteSheet(const Config& config);

        static peng::shared_ref<SpriteSheet> load_asset(const Archive& archive);

        [[nodiscard]] const std::vector<peng::shared_ref<const Sprite>>& sprites() const noexcept { return _sprites; }

    private:
        [[nodiscard]] int get_cell_count_1d(int row_length, int cell_length, int cell_padding);

        std::vector<peng::shared_ref<const Sprite>> _sprites;
    };
}
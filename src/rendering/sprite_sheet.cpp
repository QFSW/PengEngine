#include "sprite_sheet.h"

#include "texture.h"
#include "sprite.h"

using namespace rendering;
using namespace math;

std::vector<peng::shared_ref<const Sprite>> SpriteSheet::slice_grid(
    const peng::shared_ref<const Texture>& texture, const Config& config
)
{
    check(config.cell_count > 0);
    check(config.cell_size.area() > 0);

    const Vector2i cell_count_xy(
        get_cell_count_1d(texture->resolution().x, config.cell_size.x, config.cell_padding.x),
        get_cell_count_1d(texture->resolution().y, config.cell_size.y, config.cell_padding.y)
    );
    
    check(cell_count_xy.area() >= config.cell_count);
    std::vector<peng::shared_ref<const Sprite>> sprites;
    sprites.reserve(config.cell_count);

    Vector2i cursor(0, 0);
    for (int32_t y = 0; y < cell_count_xy.y; y++)
    {
        for (int32_t x = 0; x < cell_count_xy.x; x++)
        {
            sprites.push_back(peng::make_shared<Sprite>(
                texture, config.px_per_unit, cursor, config.cell_size
            ));

            // Move x cursor by one sprite + padding at a time
            cursor.x += config.cell_size.x + config.cell_padding.x;

            if (sprites.size() == config.cell_count)
            {
                break;
            }
        }

        // Reset x cursor for new row
        // Move y cursor by one sprite + padding at a time
        cursor.x = 0;
        cursor.y += config.cell_size.y + config.cell_padding.y;
    }

    return sprites;
}

int SpriteSheet::get_cell_count_1d(int row_length, int cell_length, int cell_padding)
{
    check(row_length >= cell_length);

    // Exclude the first cell, can now consider row as being one cell shorter
    // and each cell being one "padding" longer
    row_length -= cell_length;
    cell_length += cell_padding;

    check(row_length % cell_length == 0);
    return 1 + row_length / cell_length;
}

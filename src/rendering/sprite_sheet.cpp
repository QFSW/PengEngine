#include "sprite_sheet.h"

#include <core/asset.h>
#include <memory/gc.h>

#include "texture.h"
#include "sprite.h"

using namespace rendering;
using namespace math;

SpriteSheet::SpriteSheet(const Config& config)
{
    check(config.cell_count > 0);
    check(config.cell_size.area() > 0);

    const Vector2i cell_count_xy(
        get_cell_count_1d(config.texture->resolution().x, config.cell_size.x, config.cell_padding.x),
        get_cell_count_1d(config.texture->resolution().y, config.cell_size.y, config.cell_padding.y)
    );
    
    check(cell_count_xy.area() >= config.cell_count);
    _sprites.reserve(config.cell_count);

    Vector2i cursor(0, 0);
    for (int32_t y = 0; y < cell_count_xy.y; y++)
    {
        for (int32_t x = 0; x < cell_count_xy.x; x++)
        {
            _sprites.push_back(peng::make_shared<Sprite>(
                config.texture, config.px_per_unit, cursor, config.cell_size
            ));

            // Move x cursor by one sprite + padding at a time
            cursor.x += config.cell_size.x + config.cell_padding.x;

            if (_sprites.size() == config.cell_count)
            {
                break;
            }
        }

        // Reset x cursor for new row
        // Move y cursor by one sprite + padding at a time
        cursor.x = 0;
        cursor.y += config.cell_size.y + config.cell_padding.y;
    }
}

peng::shared_ref<SpriteSheet> SpriteSheet::load_asset(const Archive& archive)
{
    Asset<Texture> texture_asset(archive.read<std::string>("texture"));

    Config config {
        .texture = texture_asset.load()
    };

    archive.try_read("px_per_unit", config.px_per_unit);
    archive.try_read("cell_size", config.cell_size);
    archive.try_read("cell_padding", config.cell_padding);
    archive.try_read("cell_count", config.cell_count);

    return memory::GC::alloc<SpriteSheet>(config);
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

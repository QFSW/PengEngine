#include "sprite_sheet.h"

#include "texture.h"
#include "sprite.h"

using namespace rendering;
using namespace math;

std::vector<peng::shared_ref<const Sprite>> SpriteSheet::slice_grid(
    const peng::shared_ref<const Texture>& texture,
    float px_per_unit,
    const Vector2i& cell_size,
    int32_t cell_count
)
{
    assert(cell_count > 0);
    assert(cell_size.x > 0 && cell_size.y > 0);
    assert(texture->resolution().x % cell_size.x == 0);
    assert(texture->resolution().y % cell_size.y == 0);

    std::vector<peng::shared_ref<const Sprite>> sprites;
    const Vector2i cell_count_xy = texture->resolution() / cell_size;

    assert(cell_count_xy.area() >= cell_count);

    for (int32_t y = 0; y < cell_count_xy.y; y++)
    {
        for (int32_t x = 0; x < cell_count_xy.x; x++)
        {
            const Vector2i position(cell_size.x * x, cell_size.y * y);
            sprites.push_back(peng::make_shared<Sprite>(
                texture, px_per_unit, position, cell_size
            ));

            if (sprites.size() == cell_count)
            {
                break;
            }
        }
    }

    return sprites;
}

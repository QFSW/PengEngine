#include "bitmap_font.h"

#include <ranges>
#include <utils/utils.h>

#include "sprite.h"

using namespace rendering;

BitmapFont::BitmapFont(
    std::string&& name,
    std::unordered_map<char, peng::shared_ptr<const Sprite>>&& char_map,
    peng::shared_ptr<const Sprite>&& fallback_sprite
)
    : _name(std::move(name))
    , _char_map(std::move(char_map))
    , _fallback_sprite(std::move(fallback_sprite))
{
    check(validate_sprite(_fallback_sprite));
    for (auto& sprite : _char_map | std::views::values)
    {
        check(validate_sprite(sprite));
    }
}

BitmapFont::BitmapFont(
    const std::string& name,
    const std::unordered_map<char, peng::shared_ptr<const Sprite>>& char_map,
    const peng::shared_ptr<const Sprite>& fallback_sprite
)
    : BitmapFont(
        utils::copy(name),
        utils::copy(char_map),
        utils::copy(fallback_sprite)
    )
{ }

peng::shared_ptr<const Sprite> BitmapFont::get_sprite(char character) const
{
    if (const auto it = _char_map.find(character); it != _char_map.end())
    {
        return it->second;
    }

    return _fallback_sprite;
}

std::vector<peng::shared_ptr<const Sprite>> BitmapFont::get_sprites(const std::string& str) const
{
    std::vector<peng::shared_ptr<const Sprite>> sprites;
    for (const char c : str)
    {
        sprites.push_back(get_sprite(c));
    }

    return sprites;
}

bool BitmapFont::validate_sprite(const peng::shared_ptr<const Sprite>& sprite) const
{
    if (!sprite)
    {
        return true;
    }

    // Make sure the sprite size is almost equal to (1, 1)
    return (sprite->size() - math::Vector2f(1, 1)).magnitude_sqr() <= 0.01f;
}

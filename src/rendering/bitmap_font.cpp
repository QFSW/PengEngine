#include "bitmap_font.h"

#include <ranges>
#include <utils/utils.h>

using namespace rendering;

BitmapFont::BitmapFont(
    std::string&& name,
    std::unordered_map<char, peng::shared_ptr<const Sprite>>&& char_map,
    peng::shared_ref<const Sprite>&& fallback_sprite
)
    : _name(std::move(name))
    , _char_map(std::move(char_map))
    , _fallback_sprite(std::move(fallback_sprite))
{
    for (auto& sprite : _char_map | std::views::values)
    {
        if (!sprite)
        {
            sprite = _fallback_sprite;
        }
    }
}

BitmapFont::BitmapFont(
    const std::string& name,
    const std::unordered_map<char, peng::shared_ptr<const Sprite>>& char_map,
    const peng::shared_ref<const Sprite>& fallback_sprite
)
    : BitmapFont(
        utils::copy(name),
        utils::copy(char_map),
        utils::copy(fallback_sprite)
    )
{ }

peng::shared_ref<const Sprite> BitmapFont::get_sprite(char character) const
{
    if (const auto it = _char_map.find(character); it != _char_map.end())
    {
        return it->second.to_shared_ref();
    }

    return _fallback_sprite;
}

std::vector<peng::shared_ref<const Sprite>> BitmapFont::get_sprites(const std::string& str) const
{
    std::vector<peng::shared_ref<const Sprite>> sprites;
    for (const char c : str)
    {
        sprites.push_back(get_sprite(c));
    }

    return sprites;
}

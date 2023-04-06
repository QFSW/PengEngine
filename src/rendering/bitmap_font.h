#pragma once

#include <vector>
#include <unordered_map>

#include <memory/shared_ptr.h>

namespace rendering
{
    class Sprite;

    // A font containing a bitmap sprite for each glyph
    // All glyphs must have a physical size of (1, 1)
    // Providing nullptr for a glyph will treat it as invisible
    class BitmapFont
    {
    public:
        BitmapFont(
            std::string&& name,
            std::unordered_map<char, peng::shared_ptr<const Sprite>>&& char_map,
            peng::shared_ptr<const Sprite>&& fallback_sprite
        );

        BitmapFont(
            const std::string& name,
            const std::unordered_map<char, peng::shared_ptr<const Sprite>>& char_map,
            const peng::shared_ptr<const Sprite>& fallback_sprite
        );

        [[nodiscard]] peng::shared_ptr<const Sprite> get_sprite(char character) const;
        [[nodiscard]] std::vector<peng::shared_ptr<const Sprite>> get_sprites(const std::string& str) const;

    private:
        bool validate_sprite(const peng::shared_ptr<const Sprite>& sprite) const;

        std::string _name;
        std::unordered_map<char, peng::shared_ptr<const Sprite>> _char_map;
        peng::shared_ptr<const Sprite> _fallback_sprite;
    };
}

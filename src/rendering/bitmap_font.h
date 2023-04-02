#pragma once

#include <vector>
#include <unordered_map>

#include <memory/shared_ptr.h>

namespace rendering
{
    class Sprite;

    class BitmapFont
    {
    public:
        BitmapFont(
            std::string&& name,
            std::unordered_map<char, peng::shared_ptr<const Sprite>>&& char_map,
            peng::shared_ref<const Sprite>&& fallback_sprite
        );

        BitmapFont(
            const std::string& name,
            const std::unordered_map<char, peng::shared_ptr<const Sprite>>& char_map,
            const peng::shared_ref<const Sprite>& fallback_sprite
        );

        [[nodiscard]] peng::shared_ref<const Sprite> get_sprite(char character) const;
        [[nodiscard]] std::vector<peng::shared_ref<const Sprite>> get_sprites(const std::string& str) const;

    private:
        std::string _name;
        std::unordered_map<char, peng::shared_ptr<const Sprite>> _char_map;
        peng::shared_ref<const Sprite> _fallback_sprite;
    };
}

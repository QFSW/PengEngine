#include "text_renderer.h"

#include <components/sprite_renderer.h>
#include <rendering/bitmap_font.h>
#include <rendering/primitives.h>

IMPLEMENT_COMPONENT(components::TextRenderer);

using namespace components;
using namespace rendering;

TextRenderer::TextRenderer()
    : TextRenderer(Primitives::peng_font())
{ }

TextRenderer::TextRenderer(const peng::shared_ref<const BitmapFont>& font)
    : _font(font)
{ }

void TextRenderer::set_text(const std::string& str)
{
    if (str == _current)
    {
        return;
    }

    _current = str;

    const std::vector<peng::shared_ref<const Sprite>> sprites = _font->get_sprites(str);
    const int32_t num_chars = static_cast<int32_t>(sprites.size());

    for (int32_t char_index = 0; char_index < num_chars; char_index++)
    {
        const math::Vector3f char_pos = math::Vector3f(char_index - 0.5f * (num_chars - 1), 0, 0);

        peng::weak_ptr<Entity> char_renderer = get_nth_char(char_index);
        char_renderer->get_component<SpriteRenderer>()->sprite() = sprites[char_index];
        char_renderer->local_transform().position = char_pos;
        char_renderer->set_active(true);
    }

    // Disable characters not in use 
    for (int32_t char_index = num_chars; char_index < _chars.size(); char_index++)
    {
        get_nth_char(char_index)->set_active(false);
    }
}

peng::weak_ptr<Entity> TextRenderer::get_nth_char(int32_t n)
{
    if (n < _chars.size())
    {
        return _chars[n];
    }

    if (!_char_root)
    {
        _char_root = owner().create_child<Entity>("Chars");
    }

    peng::weak_ptr<Entity> char_renderer = _char_root->create_child<Entity>(
        strtools::catf("Char#%d", n)
    );

    char_renderer->add_component<SpriteRenderer>();
    _chars.push_back(char_renderer);

    return char_renderer;
}

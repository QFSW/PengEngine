#include "text_renderer.h"

#include <entities/camera.h>
#include <rendering/render_queue.h>
#include <rendering/bitmap_font.h>
#include <rendering/primitives.h>

IMPLEMENT_COMPONENT(components::TextRenderer);

using namespace components;
using namespace entities;
using namespace rendering;
using namespace math;

TextRenderer::TextRenderer()
    : TextRenderer(Primitives::peng_font())
{ }

TextRenderer::TextRenderer(const peng::shared_ref<const BitmapFont>& font)
    : Component(TickGroup::render_parallel)
    , _font(font)
{ }

void TextRenderer::tick(float delta_time)
{
    Component::tick(delta_time);

    if (!Camera::current())
    {
        return;
    }

    const Matrix4x4f view_matrix = Camera::current()->view_matrix();
    const Matrix4x4f model_matrix = owner().transform_matrix();
    const Matrix4x4f mvp_matrix = view_matrix * model_matrix;

    for (const GlyphData& glyph : _current_glyphs)
    {
        RenderQueue::get().enqueue_command(SpriteDrawCall{
            .sprite = glyph.sprite,
            .mvp_matrix = mvp_matrix * glyph.transform
        });
    }
}

void TextRenderer::set_text(const std::string& str)
{
    if (str == _current)
    {
        return;
    }

    _current = str;
    _current_glyphs.clear();

    const std::vector<peng::shared_ptr<const Sprite>> sprites = _font->get_sprites(str);
    const int32_t num_chars = static_cast<int32_t>(sprites.size());

    for (int32_t char_index = 0; char_index < num_chars; char_index++)
    {
        // Only add a glyph if this character is visible
        if (sprites[char_index])
        {
            const Vector3f char_pos = Vector3f(char_index - 0.5f * (num_chars - 1), 0, 0);

            _current_glyphs.push_back(GlyphData{
                .sprite = sprites[char_index].to_shared_ref(),
                .transform = Matrix4x4f::from_translation(char_pos)
            });
        }
    }
}

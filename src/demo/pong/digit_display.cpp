#include "digit_display.h"

#include <components/mesh_renderer.h>
#include <rendering/primitives.h>

#include "rendering/material.h"

IMPLEMENT_ENTITY(demo::pong::DigitDisplay);

using namespace demo::pong;

DigitDisplay::DigitDisplay(const std::string& name)
    : Entity(name, TickGroup::none)
{
    add_component<components::MeshRenderer>(
        rendering::Primitives::quad(),
        rendering::Primitives::unlit_material()
    );
}

void DigitDisplay::post_create()
{
    Entity::post_create();

    if (_digit_textures.size() == 10 && _current == 0xFF)
    {
        set_digit(0);
    }
}

void DigitDisplay::set_digit_textures(const std::vector<peng::shared_ref<const rendering::Texture>>& textures)
{
    assert(textures.size() == 10);
    _digit_textures = textures;
}

void DigitDisplay::set_digit(uint8_t digit)
{
    if (digit == _current)
    {
        return;
    }

    _current = digit;

    assert(_digit_textures.size() == 10);
    assert(digit < _digit_textures.size());

    get_component<components::MeshRenderer>()->material()->set_parameter("color_tex", _digit_textures[digit]);

}

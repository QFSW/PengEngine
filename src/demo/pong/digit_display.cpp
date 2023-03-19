#include "digit_display.h"

#include <components/sprite_renderer.h>
#include <rendering/primitives.h>

#include "rendering/material.h"

IMPLEMENT_ENTITY(demo::pong::DigitDisplay);

using namespace demo::pong;

DigitDisplay::DigitDisplay(const std::string& name)
    : Entity(name, TickGroup::none)
{
    add_component<components::SpriteRenderer>();
}

void DigitDisplay::post_create()
{
    Entity::post_create();

    if (_digit_sprites.size() == 10 && _current == 0xFF)
    {
        set_digit(0);
    }
}

void DigitDisplay::set_digit_sprites(const std::vector<peng::shared_ref<const rendering::Sprite>>& sprites)
{
    assert(sprites.size() == 10);
    _digit_sprites = sprites;
}

void DigitDisplay::set_digit(uint8_t digit)
{
    if (digit == _current)
    {
        return;
    }

    _current = digit;

    assert(_digit_sprites.size() == 10);
    assert(digit < _digit_sprites.size());

    get_component<components::SpriteRenderer>()->sprite() = _digit_sprites[digit];

}

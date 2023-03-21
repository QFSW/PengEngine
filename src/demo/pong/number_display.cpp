#include "number_display.h"

#include "digit_display.h"

IMPLEMENT_ENTITY(demo::pong::NumberDisplay);

using namespace demo::pong;

void NumberDisplay::post_create()
{
    Entity::post_create();

    if (_digit_sprites.size() == 10 && _current == -1)
    {
        set_number(0);
    }
}

void NumberDisplay::set_digit_sprites(const std::vector<peng::shared_ref<const rendering::Sprite>>& sprites)
{
    assert(sprites.size() == 10);
    _digit_sprites = sprites;

    for (peng::weak_ptr<DigitDisplay> digit : _digits)
    {
        digit->set_digit_sprites(_digit_sprites);
    }
}

void NumberDisplay::set_number(int32_t number)
{
    if (number == _current)
    {
        return;
    }

    _current = number;

    // Decompose number into digits
    std::vector<uint8_t> digit_values;
    while (number > 0)
    {
        digit_values.push_back(number % 10);
        number /= 10;
    }

    if (digit_values.empty())
    {
        digit_values.push_back(0);
    }

    // Get all digits needed and set values
    const int32_t num_digits = static_cast<int32_t>(digit_values.size());
    for (int32_t digit_index = 0; digit_index < num_digits; digit_index++)
    {
        const math::Vector3f digit_position = math::Vector3f(0.5f * (num_digits - 1) - digit_index, 0, 0);

        peng::weak_ptr<DigitDisplay> digit = get_nth_digit(digit_index);
        digit->set_digit(digit_values[digit_index]);
        digit->local_transform().position = digit_position;
        digit->set_active(true);
    }

    // Disable digits not in use 
    for (int32_t digit_index = num_digits; digit_index < _digits.size(); digit_index++)
    {
        get_nth_digit(digit_index)->set_active(false);
    }
}

peng::weak_ptr<DigitDisplay> NumberDisplay::get_nth_digit(int32_t n)
{
    if (n < _digits.size())
    {
        return _digits[n];
    }

    peng::weak_ptr<DigitDisplay> digit = EntitySubsystem::get().create_entity<DigitDisplay>(
        strtools::catf("Digit%d", n)
    );

    digit->set_digit_sprites(_digit_sprites);
    digit->set_parent(weak_this());
    _digits.push_back(digit);

    return digit;
}


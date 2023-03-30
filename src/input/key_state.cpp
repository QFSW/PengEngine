#include "key_state.h"

using namespace input;

bool KeyState::pressed() const noexcept
{
    return _is_down && !_was_down;
}

bool KeyState::released() const noexcept
{
    return !_is_down && _was_down;
}

bool KeyState::is_down() const noexcept
{
    return _is_down;
}

bool KeyState::was_down() const noexcept
{
    return _was_down;
}

void KeyState::update(bool is_down) noexcept
{
    _was_down = _is_down;
    _is_down = is_down;
}

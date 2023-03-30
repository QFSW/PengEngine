#include "layer.h"

#include <utils/check.h>

using namespace physics;

Layer::Layer()
    : _index(0)
{ }

Layer::Layer(uint8_t index)
    : _index(index)
{
    check(_index <= 63);
}

uint8_t Layer::index() const noexcept
{
    return _index;
}

uint64_t Layer::mask() const noexcept
{
    return 0x1ULL << _index;
}

bool Layer::operator==(const Layer& other) const noexcept
{
    return _index == other._index;
}

bool Layer::operator!=(const Layer& other) const noexcept
{
    return !(*this == other);
}

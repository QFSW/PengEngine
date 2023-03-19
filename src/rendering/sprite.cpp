#include "sprite.h"

#include "texture.h"

using namespace rendering;

Sprite::Sprite(const peng::shared_ref<const Texture>& texture, float px_per_unit)
    : _texture(texture)
    , _px_per_unit(px_per_unit)
{ }

peng::shared_ref<const Texture> Sprite::texture() const noexcept
{
    return _texture;
}

float Sprite::px_per_unit() const noexcept
{
    return _px_per_unit;
}

math::Vector2f Sprite::size() const
{
    return _texture->resolution() / _px_per_unit;
}

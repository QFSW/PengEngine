#include "sprite.h"

#include <core/asset.h>

#include "texture.h"

using namespace rendering;
using namespace math;

Sprite::Sprite(const peng::shared_ref<const Texture>& texture, float px_per_unit)
    : Sprite(texture, px_per_unit, Vector2i::zero(), texture->resolution())
{ }

Sprite::Sprite(
    const peng::shared_ref<const Texture>& texture,
    float px_per_unit,
    const Vector2i& position,
    const Vector2i& resolution
)
    : _texture(texture)
    , _px_per_unit(px_per_unit)
    , _position(position)
    , _resolution(resolution)
{
    [[maybe_unused]] const Vector2i texture_res = _texture->resolution();
    check(_position.x >= 0 && _position.y >= 0);
    check(_position.x < texture_res.x && _position.y < texture_res.y);
    check(_resolution.x >= 0 && _resolution.y >= 0);
    check(_position.x + _resolution.x <= texture_res.x && _position.y + _resolution.y <= texture_res.y);
}

peng::shared_ref<Sprite> Sprite::load_asset(const Archive& archive)
{
    Asset<Texture> texture_asset(archive.read<std::string>("texture"));
    peng::shared_ref<const Texture> texture = texture_asset.load();

    const float px_per_unit = archive.read<float>("px_per_unit");
    const Vector2i position = archive.read_or("position", Vector2i::zero());
    const Vector2i resolution = archive.read_or("resolution", texture->resolution());

    return peng::make_shared<Sprite>(texture, px_per_unit, position, resolution);
}

peng::shared_ref<const Texture> Sprite::texture() const noexcept
{
    return _texture;
}

float Sprite::px_per_unit() const noexcept
{
    return _px_per_unit;
}

const math::Vector2i& Sprite::position() const
{
    return _position;
}

const Vector2i& Sprite::resolution() const
{
    return _resolution;
}

math::Vector2f Sprite::size() const
{
    return _resolution / _px_per_unit;
}

TransparencyMode Sprite::transparency() const noexcept
{
    return _texture->transparency();
}

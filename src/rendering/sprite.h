#pragma once

#include <memory/shared_ref.h>
#include <math/vector2.h>

namespace rendering
{
    class Texture;

    class Sprite
    {
    public:
        Sprite(const peng::shared_ref<const Texture>& texture, float px_per_unit);

        [[nodiscard]] peng::shared_ref<const Texture> texture() const noexcept;
        [[nodiscard]] float px_per_unit() const noexcept;
        [[nodiscard]] math::Vector2f size() const;

    private:
        peng::shared_ref<const Texture> _texture;
        float _px_per_unit;
    };
}
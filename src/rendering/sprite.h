#pragma once

#include <memory/shared_ref.h>
#include <math/vector2.h>

struct Archive;

namespace rendering
{
    class Texture;

    class Sprite
    {
    public:
        Sprite(const peng::shared_ref<const Texture>& texture, float px_per_unit);
        Sprite(
            const peng::shared_ref<const Texture>& texture,
            float px_per_unit,
            const math::Vector2i& position,
            const math::Vector2i& resolution
        );

        static peng::shared_ref<Sprite> load_asset(const Archive& archive);

        [[nodiscard]] peng::shared_ref<const Texture> texture() const noexcept;
        [[nodiscard]] float px_per_unit() const noexcept;
        [[nodiscard]] const math::Vector2i& position() const;
        [[nodiscard]] const math::Vector2i& resolution() const;
        [[nodiscard]] math::Vector2f size() const;

    private:
        peng::shared_ref<const Texture> _texture;
        float _px_per_unit;
        math::Vector2i _position;
        math::Vector2i _resolution;
    };
}
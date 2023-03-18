#pragma once

#include "vector3.h"
#include "matrix4x4.h"

namespace math
{
    struct Transform
    {
        Vector3f position;
        Vector3f scale;
        Vector3f rotation;

        Transform();
        Transform(const Vector3f& position, const Vector3f& scale, const Vector3f& rotation);

        [[nodiscard]] Matrix4x4f to_matrix() const noexcept;
        [[nodiscard]] Matrix4x4f to_inverse_matrix() const noexcept;

        [[nodiscard]] Vector3f local_right() const noexcept;
        [[nodiscard]] Vector3f local_up() const noexcept;
        [[nodiscard]] Vector3f local_forwards() const noexcept;
    };
}

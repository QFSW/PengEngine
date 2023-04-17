#pragma once

#include <memory/shared_ptr.h>
#include <math/matrix4x4.h>

namespace rendering
{
    class Sprite;
    class Material;

    // Sprite draw calls are specialized draw calls for 2D sprites
    // They should be used over regular calls as they allow the sprite batcher
    // to automatically batch sprites into less draw calls when possible
    struct SpriteDrawCall
    {
        peng::shared_ref<const Sprite> sprite;
        math::Matrix4x4f model_matrix;
        math::Matrix4x4f view_matrix;
        math::Vector4f color = math::Vector4f::one();

        // TODO: add a material override
    };
}
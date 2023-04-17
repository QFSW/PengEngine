#pragma once

#include <memory/shared_ptr.h>

namespace rendering
{
    class Mesh;
    class Material;

    // Draw calls specify an object to draw and its corresponding material
    // They should be used instead of drawing objects directly to allow the
    // draw call tree to automatically sort draw calls minimize state switches
    // for maximum efficiency
    struct DrawCall
    {
        peng::shared_ptr<const Mesh> mesh;
        peng::shared_ptr<Material> material;
        float order = 0;
        int32_t instance_count = 1;
    };
}

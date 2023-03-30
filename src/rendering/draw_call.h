#pragma once

#include <memory/shared_ptr.h>

namespace rendering
{
    class Mesh;
    class Material;

    struct DrawCall
    {
        peng::shared_ptr<const Mesh> mesh;
        peng::shared_ptr<Material> material;
        float order = 0;
    };
}

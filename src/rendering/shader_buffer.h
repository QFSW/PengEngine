#pragma once

#include <GL/glew.h>

namespace rendering
{
    class IShaderBuffer
    {
    public:
        IShaderBuffer() = default;
        virtual ~IShaderBuffer() = default;

        // Gets the SSBO for this shader buffer
        // Requires that data has already been created
        [[nodiscard]] virtual GLuint get_ssbo() const = 0;
    };
}

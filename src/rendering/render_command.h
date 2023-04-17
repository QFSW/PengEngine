#pragma once

#include <variant>

#include "draw_call.h"
#include "sprite_draw_call.h"

namespace rendering
{
    using RenderCommand = std::variant<
        DrawCall,
        SpriteDrawCall
    >;
}
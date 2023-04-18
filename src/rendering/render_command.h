#pragma once

#include <variant>

#include "draw_call.h"
#include "sprite_draw_call.h"

namespace rendering
{
    // Dummy null op render command
    struct RenderCommandNullOp {};

    using RenderCommand = std::variant<
        RenderCommandNullOp,
        DrawCall,
        SpriteDrawCall
    >;
}
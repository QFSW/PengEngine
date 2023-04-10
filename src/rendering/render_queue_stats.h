#pragma once

#include <cstdint>

namespace rendering
{
    // Various stats about a completed render queue
    struct RenderQueueStats
    {
        int32_t draw_calls = 0;
        int32_t triangles = 0;
        int32_t shader_switches = 0;
        int32_t mesh_switches = 0;
    };
}

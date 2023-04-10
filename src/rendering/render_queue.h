#pragma once

#include <vector>

#include <common/common.h>
#include <utils/singleton.h>

#include "draw_call.h"
#include "render_queue_stats.h"

namespace rendering
{
    class RenderQueue : public utils::Singleton<RenderQueue>
    {
        using Singleton::Singleton;

    public:
        // Executes all items in the render queue
        void execute();
        void enqueue_draw(DrawCall&& draw_call);

        // Various stats about the render queue from the previous frame
        [[nodiscard]] const RenderQueueStats& last_frame_stats() const noexcept;

    private:
        void drain_queues();

        std::vector<DrawCall> _draw_calls;
        common::concurrent_queue<DrawCall> _draw_call_queue;
        RenderQueueStats _queue_stats;
    };
}
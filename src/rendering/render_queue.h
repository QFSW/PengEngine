#pragma once

#include <vector>

#include <common/common.h>
#include <utils/singleton.h>

#include "render_command.h"
#include "render_queue_stats.h"
#include "sprite_batcher.h"

namespace rendering
{
    class RenderQueue : public utils::Singleton<RenderQueue>
    {
        using Singleton::Singleton;

    public:
        // Executes all items in the render queue
        void execute();

        // Enqueues a render command to the queue
        void enqueue_command(RenderCommand&& command);

        // Various stats about the render queue from the previous frame
        [[nodiscard]] const RenderQueueStats& last_frame_stats() const noexcept;

    private:
        void drain_queues();
        void consume_command(RenderCommand& command);

        SpriteBatcher _sprite_batcher;
        std::vector<DrawCall> _draw_calls;
        std::vector<SpriteDrawCall> _sprite_draw_calls;
        common::concurrent_queue<RenderCommand> _command_queue;
        RenderQueueStats _queue_stats;
    };
}

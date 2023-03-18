#pragma once

#include <vector>

#include <common/common.h>
#include <utils/singleton.h>

#include "draw_call.h"

namespace rendering
{
    class RenderQueue : public utils::Singleton<RenderQueue>
    {
        using Singleton::Singleton;

    public:
        // Executes all items in the render queue
        void execute();
        void enqueue_draw(DrawCall&& draw_call);

    private:
        void drain_queues();

        std::vector<DrawCall> _draw_calls;
        common::concurrent_queue<DrawCall> _draw_call_queue;
    };
}
#include "render_queue.h"

#include <profiling/scoped_event.h>

#include "draw_call_tree.h"

using namespace rendering;

void RenderQueue::execute()
{
    SCOPED_EVENT("RenderQueue - execute");
    drain_queues();

    const DrawCallTree tree(std::move(_draw_calls));
    tree.execute();
}

void RenderQueue::enqueue_draw(DrawCall&& draw_call)
{
    _draw_call_queue.enqueue(draw_call);
}

void RenderQueue::drain_queues()
{
    DrawCall draw_call;
    while (_draw_call_queue.try_dequeue(draw_call))
    {
        _draw_calls.push_back(draw_call);
    }
}


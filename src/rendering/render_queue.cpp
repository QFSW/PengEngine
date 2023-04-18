#include "render_queue.h"

#include <profiling/scoped_event.h>
#include <utils/functional.h>

#include "texture_binding_cache.h"
#include "draw_call_tree.h"

using namespace rendering;

void RenderQueue::execute()
{
    SCOPED_EVENT("RenderQueue - execute");
    RenderQueueStats stats;

    drain_queues();

    _sprite_batcher.convert_draws(_sprite_draw_calls, _draw_calls);
    _sprite_draw_calls.clear();

    const DrawCallTree tree(std::move(_draw_calls));
    tree.execute(stats);

    // TODO: for some reason the texture binding cache breaks after pause if you don't clear it
    TextureBindingCache::get().unbind_all();
    _queue_stats = stats;
}

void RenderQueue::enqueue_command(RenderCommand&& command)
{
    _command_queue.enqueue(command);
}

const RenderQueueStats& RenderQueue::last_frame_stats() const noexcept
{
    return _queue_stats;
}

void RenderQueue::drain_queues()
{
    RenderCommand command;
    while (_command_queue.try_dequeue(command))
    {
        consume_command(command);
    }
}

void RenderQueue::consume_command(RenderCommand& command)
{
    std::visit(functional::overload{
        [&](DrawCall& x) { _draw_calls.push_back(std::move(x)); },
        [&](SpriteDrawCall& x) { _sprite_draw_calls.push_back(std::move(x)); }
    }, command);
}


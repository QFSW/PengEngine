#include "render_queue.h"

#include <profiling/scoped_event.h>
#include <utils/functional.h>
#include <utils/strtools.h>

#include "texture_binding_cache.h"
#include "draw_call_tree.h"

using namespace rendering;

RenderQueue::RenderQueue()
    : _command_queue_consumer(_command_queue)
    , _command_buffer_size(16)
    , _last_command_buffer_usage(0)
{ }

void RenderQueue::execute()
{
    SCOPED_EVENT("RenderQueue - execute");
    RenderQueueStats stats;

    flush_queue();

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

void RenderQueue::flush_queue()
{
    SCOPED_EVENT("RenderQueue - flush queue");

    size_t peak_buffer_usage = 0;
    _command_buffer.resize(_command_buffer_size, RenderCommandNullOp());

    // Keep dequeuing in bulk until the queue is empty
    while (const size_t command_count = _command_queue.try_dequeue_bulk(
        _command_queue_consumer,
        _command_buffer.begin(),
        _command_buffer_size
    ))
    {
        SCOPED_EVENT("RenderQueue - consume commands", strtools::catf_temp("%d commands", command_count));
        peak_buffer_usage = std::max(peak_buffer_usage, command_count);

        // If the dequeue saturated our buffer, increase the size by 50% on the next dequeue
        if (command_count == _command_buffer_size)
        {
            _command_buffer_size = static_cast<size_t>(std::ceil(static_cast<double>(_command_buffer_size) * 1.5));
            _command_buffer.resize(_command_buffer_size, RenderCommandNullOp());
        }

        for (size_t i = 0; i < command_count; i++)
        {
            consume_command(_command_buffer[i]);
        }
    }

    // We don't clear the buffer each frame since initializing it with null ops is expensive
    // To avoid extending lifetime of resources via commands in the buffer
    // We can compare this frames peak usage to the previous and only clear those commands
    // This way we don't thrash the buffer when it's being constantly overwritten
    for (size_t i = peak_buffer_usage; i < _last_command_buffer_usage; i++)
    {
        _command_buffer[i] = RenderCommandNullOp();
    }

    _last_command_buffer_usage = peak_buffer_usage;
}

void RenderQueue::consume_command(RenderCommand& command)
{
    std::visit(functional::overload{
        [&](RenderCommandNullOp&) { /* Do nothing */ },
        [&](DrawCall& x) { _draw_calls.push_back(std::move(x)); },
        [&](SpriteDrawCall& x) { _sprite_draw_calls.push_back(std::move(x)); }
    }, command);
}


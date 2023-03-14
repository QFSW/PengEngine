#include "render_queue.h"

#include <profiling/scoped_event.h>

#include "draw_call_tree.h"

using namespace rendering;

void RenderQueue::execute()
{
	SCOPED_EVENT("RenderQueue - execute");

	const DrawCallTree tree(_draw_calls);
	tree.execute();

	_draw_calls.clear();
}

void RenderQueue::enqueue_draw(DrawCall&& draw_call)
{
	_draw_calls.push_back(draw_call);
}

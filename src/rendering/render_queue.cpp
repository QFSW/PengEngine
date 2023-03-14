#include "render_queue.h"

#include <profiling/scoped_event.h>

#include "mesh.h"
#include "material.h"

using namespace rendering;

void RenderQueue::execute()
{
	SCOPED_EVENT("RenderQueue - execute");

	for (const DrawCall& draw_call : _draw_calls)
	{
		draw_call.material->use();
		draw_call.mesh->render();
	}

	_draw_calls.clear();
}

void RenderQueue::enqueue_draw(DrawCall&& draw_call)
{
	_draw_calls.push_back(draw_call);
}

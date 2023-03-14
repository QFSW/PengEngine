#pragma once

#include <vector>

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
		std::vector<DrawCall> _draw_calls;
	};
}
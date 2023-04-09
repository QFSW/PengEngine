#include "debug_entity.h"

#include <core/peng_engine.h>
#include <input/input_subsystem.h>
#include <rendering/window_subsystem.h>

IMPLEMENT_ENTITY(demo::DebugEntity);

using namespace demo;
using namespace input;
using namespace rendering;

void DebugEntity::tick(float delta_time)
{
	Entity::tick(delta_time);

	if (InputSubsystem::get()[KeyCode::num_row_0].pressed())
	{
		EntitySubsystem::get().dump_hierarchy();
	}

	if (InputSubsystem::get()[KeyCode::f11].pressed())
	{
		WindowSubsystem::get().toggle_fullscreen();
	}

	if (InputSubsystem::get()[KeyCode::f10].pressed())
	{
		PengEngine::get().request_shutdown();
	}
}

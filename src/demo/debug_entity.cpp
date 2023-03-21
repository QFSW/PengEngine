#include "debug_entity.h"

#include <core/peng_engine.h>

IMPLEMENT_ENTITY(demo::DebugEntity);

void demo::DebugEntity::tick(float delta_time)
{
	Entity::tick(delta_time);

	using namespace input;
	const InputManager& input_manager = PengEngine::get().input_manager();

	if (input_manager[KeyCode::num_row_0].pressed())
	{
		EntitySubsystem::get().dump_hierarchy();
	}

	if (input_manager[KeyCode::f11].pressed())
	{
		PengEngine::get().toggle_fullscreen();
	}

	if (input_manager[KeyCode::escape].pressed())
	{
		PengEngine::get().request_shutdown();
	}
}

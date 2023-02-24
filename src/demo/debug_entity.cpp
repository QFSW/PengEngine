#include "debug_entity.h"

IMPLEMENT_ENTITY(demo::DebugEntity);

void demo::DebugEntity::tick(float delta_time)
{
	Entity::tick(delta_time);

	using namespace input;
	const InputManager& input_manager = PengEngine::get().input_manager();

	if (input_manager[KeyCode::num_row_0].pressed())
	{
		PengEngine::get().entity_manager().dump_hierarchy();
	}

	if (input_manager[KeyCode::f11].pressed())
	{
		PengEngine::get().maximize_window();
	}

	if (input_manager[KeyCode::escape].pressed())
	{
		PengEngine::get().request_shutdown();
	}
}

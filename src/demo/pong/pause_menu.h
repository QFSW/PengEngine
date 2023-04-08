#pragma once

#include <core/entity.h>

namespace demo::pong
{
	class PauseMenu final : public Entity
	{
		DECLARE_ENTITY(PauseMenu);

		DEFINE_EVENT(on_resume)
		DEFINE_EVENT(on_restart)
		DEFINE_EVENT(on_quit)

	public:
		PauseMenu();

		void post_create() override;
		void tick(float delta_time) override;

		void show();
		void hide();

		float pause_size = 1;
		float btn_size = 1;

	private:
		void update_buttons();

		int32_t _selected_entry = 0;
		std::vector<peng::weak_ptr<Entity>> _buttons;
		std::vector<utils::Event<>*> _button_events;
	};
}

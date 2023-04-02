#pragma once

#include <core/entity.h>

namespace audio
{
	class AudioClip;
}

namespace demo::pong
{
	class PengPong final : public Entity
	{
		DECLARE_ENTITY(PengPong);

	public:
		using Entity::Entity;

		void post_create() override;
		void tick(float delta_time) override;

	private:
		enum class GameState
		{
		    playing,
			paused
		};

		void load_resources();
		void build_camera();
		void build_world();

		void pause();
		void unpause();

		GameState _game_state = GameState::playing;
		peng::weak_ptr<Entity> _world_root;
		peng::weak_ptr<Entity> _pause_root;
		peng::shared_ptr<audio::AudioClip> _bounce_wall_sfx;
		peng::shared_ptr<audio::AudioClip> _bounce_paddle_sfx;
		peng::shared_ptr<audio::AudioClip> _goal_sfx;

		static constexpr float ortho_size = 20;
		static constexpr float paddle_margin = 3;
		static constexpr float digit_size = 5;
	};
}

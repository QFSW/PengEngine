#pragma once

#include <core/entity.h>
#include <audio/audio_pool.h>

namespace components
{
	class Collider2D;
}

namespace audio
{
	class AudioClip;
}

namespace demo::pong
{
	class Ball final : public Entity
	{
		DECLARE_ENTITY(Ball);

	public:
		Ball();

	private:
		void respawn();
		void handle_collision(peng::weak_ptr<components::Collider2D> collider);

		float _speed;

		peng::shared_ref<audio::AudioClip> _bounce_wall_sfx;
		peng::shared_ref<audio::AudioClip> _bounce_paddle_sfx;
		peng::shared_ref<audio::AudioClip> _goal_sfx;
		audio::AudioPool _audio_pool;
	};
}

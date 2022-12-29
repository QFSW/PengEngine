#pragma once

#include <core/entity.h>
#include <math/transform.h>
#include <memory/weak_ptr.h>

namespace entities
{
	class Camera final : public Entity
	{
	public:
		explicit Camera(float ortho_size, float near_clip, float far_clip);

		static peng::weak_ptr<Camera>& current();

		void post_create() override;
		void tick(double delta_time) override;

		[[nodiscard]] math::Transform& transform() noexcept;
		[[nodiscard]] const math::Transform& transform() const noexcept;
		[[nodiscard]] const math::Matrix4x4f& view_matrix() const noexcept;

	private:
		static peng::weak_ptr<Camera> _current;

		float _ortho_size;
		float _near_clip;
		float _far_clip;
		math::Transform _transform;
		math::Matrix4x4f _view_matrix;
	};
}

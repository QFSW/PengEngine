#pragma once

#include <core/entity.h>
#include <math/transform.h>
#include <memory/weak_ptr.h>

namespace entities
{
	class Camera final : public Entity
	{
	public:
		enum class Projection
		{
			orthographic,
			perspective
		};

		Camera();

		static peng::weak_ptr<Camera>& current();

		void post_create() override;
		void tick(double delta_time) override;

		void make_perspective(float fov, float near_clip, float far_clip);
		void make_orthographic(float ortho_size, float near_clip, float far_clip);

		math::Transform& transform() noexcept;
		float& ortho_size() noexcept;

		[[nodiscard]] const math::Transform& transform() const noexcept;
		[[nodiscard]] const math::Matrix4x4f& view_matrix() const noexcept;
		[[nodiscard]] Projection projection() const noexcept;

	private:
		static peng::weak_ptr<Camera> _current;

		void validate_config() const noexcept;
		[[nodiscard]] math::Matrix4x4f calc_projection_matrix();

		float _fov;
		float _ortho_size;
		float _near_clip;
		float _far_clip;
		Projection _projection;
		math::Transform _transform;
		math::Matrix4x4f _view_matrix;
	};
}

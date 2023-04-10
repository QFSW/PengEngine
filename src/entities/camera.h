#pragma once

#include <core/entity.h>
#include <math/transform.h>
#include <memory/weak_ptr.h>

namespace entities
{
	class Camera final : public Entity
	{
		DECLARE_ENTITY(Camera);

	public:
		enum class Projection
		{
			orthographic,
			perspective
		};

		enum class PixelPerfectMode
		{
		    nearest,
			increase_size,
			decrease_size
		};

		Camera();
		explicit Camera(const std::string& name);
		explicit Camera(std::string&& name);

		static const peng::weak_ptr<Camera>& current();

		void post_create() override;
		void tick(float delta_time) override;

		void make_perspective(float fov, float near_clip, float far_clip);
		void make_orthographic(float ortho_size, float near_clip, float far_clip);

		// Specifies if pixel perfect rendering should be used (orthographic only)
		// If enabled, then all sprites with a location/size that is some multiple of the specified basis
		// Are guaranteed to be rendered pixel perfect regardless of the viewport resolution
		void set_pixel_perfect(bool enabled, float basis = 1, PixelPerfectMode mode = PixelPerfectMode::nearest);

		float& ortho_size() noexcept;

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
		bool _pixel_perfect_enabled;
		float _pixel_perfect_basis;
		PixelPerfectMode _pixel_perfect_mode;
		Projection _projection;
		math::Matrix4x4f _view_matrix;
	};
}

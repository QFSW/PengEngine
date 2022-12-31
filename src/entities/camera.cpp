#include "camera.h"

#include <cassert>
#include <numbers>

#include <core/logger.h>
#include <core/peng_engine.h>

using namespace entities;
using namespace math;

peng::weak_ptr<Camera> Camera::_current;

Camera::Camera()
	: Entity(true)
	, _fov(70)
	, _ortho_size(20)
	, _near_clip(0.01f)
	, _far_clip(1000.0f)
	, _projection(Projection::perspective)
	, _view_matrix(Matrix4x4f::identity())
{ }

peng::weak_ptr<Camera>& Camera::current()
{
	return _current;
}

void Camera::post_create()
{
	Entity::post_create();

	if (_current.valid())
	{
		Logger::get().log(LogSeverity::warning, "Camera entity created when a valid camera already exists");
	}

	_current = peng::weak_ptr<Camera>(std::static_pointer_cast<Camera>(shared_from_this()));
	assert(_current);
}

void Camera::tick(float delta_time)
{
	Entity::tick(delta_time);

	static const Matrix4x4f z_reverse =
		Matrix4x4f::identity()
		.scaled(Vector3f(1, 1, -1));

	_view_matrix = calc_projection_matrix() * z_reverse * _transform.to_inverse_matrix();
}

void Camera::make_perspective(float fov, float near_clip, float far_clip)
{
	_projection = Projection::perspective;
	_fov = fov;
	_near_clip = near_clip;
	_far_clip = far_clip;

	validate_config();
}

void Camera::make_orthographic(float ortho_size, float near_clip, float far_clip)
{
	_projection = Projection::orthographic;
	_ortho_size = ortho_size;
	_near_clip = near_clip;
	_far_clip = far_clip;

	validate_config();
}

float& Camera::ortho_size() noexcept
{
	return _ortho_size;
}

const Matrix4x4f& Camera::view_matrix() const noexcept
{
	return _view_matrix;
}

Camera::Projection Camera::projection() const noexcept
{
	return _projection;
}

void Camera::validate_config() const noexcept
{
	if (_near_clip >= _far_clip)
	{
		Logger::get().logf(LogSeverity::warning,
			"Camera far-clip (%f) is not grater than near-clip (%f) - unexpected results may occur",
			_far_clip, _near_clip
		);
	}
}

Matrix4x4f Camera::calc_projection_matrix()
{
	const Vector2f resolution = PengEngine::get().resolution();
	const float aspect_ratio = resolution.x / resolution.y;

	if (_projection == Projection::orthographic)
	{
		Transform ortho_transform;
		ortho_transform.position = Vector3f(0, 0, _near_clip);
		ortho_transform.scale = Vector3f(_ortho_size * aspect_ratio, _ortho_size, _far_clip - _near_clip);

		Vector3f& scale = _transform.scale;
		if (scale.x * scale.y * scale.z == 0.0f)
		{
			Logger::get().logf(LogSeverity::warning,
				"Camera scale of (%f, %f, %f) is invalid - reverting to (1, 1, 1)",
				scale.x, scale.y, scale.z
			);

			scale = Vector3f::one();
		}

		return ortho_transform.to_inverse_matrix();
	}

	if (_projection == Projection::perspective)
	{
		const float far = _far_clip;
		const float near = _near_clip;
		const float fov_rads = _fov / 180 * std::numbers::pi_v<float>;
		const float f = 1 / std::tan(fov_rads / 2);

		return Matrix4x4f({
			f / aspect_ratio, 0, 0,                                0,
			0,                f, 0,                                0,
			0,                0, -(far + near) / (far - near),     -1,
			0,                0, (far * near * -2) / (far - near), 0,
		});
	}

	Logger::get().logf(LogSeverity::error, "Unsupported projection mode %d", _projection);
	return Matrix4x4f::identity();
}

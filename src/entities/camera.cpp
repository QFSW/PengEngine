#include "camera.h"

#include <cassert>

#include <core/logger.h>
#include <core/peng_engine.h>

using namespace entities;
using namespace math;

peng::weak_ptr<Camera> Camera::_current;

Camera::Camera(float ortho_size, float near_clip, float far_clip)
	: Entity(true)
	, _ortho_size(ortho_size)
	, _near_clip(near_clip)
	, _far_clip(far_clip)
	, _view_matrix(Matrix4x4f::identity())
{
	if (_near_clip >= _far_clip)
	{
		Logger::get().logf(LogSeverity::warning,
			"Camera far-clip (%f) is not grater than near-clip (%f) - unexpected results may occur",
			_far_clip, _near_clip
		);
	}
}

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

void Camera::tick(double delta_time)
{
	Entity::tick(delta_time);

	const Vector2f resolution = PengEngine::get().resolution();
	const float aspect_ratio = resolution.x / resolution.y;

	Transform ortho_transform;
	ortho_transform.position = Vector3f(0, 0, _near_clip);
	ortho_transform.scale = Vector3f(_ortho_size, _ortho_size / aspect_ratio, _far_clip - _near_clip);

	Vector3f& scale = _transform.scale;
	if (scale.x * scale.y * scale.z == 0.0f)
	{
		Logger::get().logf(LogSeverity::warning,
			"Camera scale of (%f, %f, %f) is invalid - reverting to (1, 1, 1)",
			scale.x, scale.y, scale.z
		);

		scale = Vector3f::one();
	}

	_view_matrix = _transform.to_inverse_matrix() * ortho_transform.to_inverse_matrix();
}

Transform& Camera::transform() noexcept
{
	return _transform;
}

const Transform& Camera::transform() const noexcept
{
	return _transform;
}

const Matrix4x4f& Camera::view_matrix() const noexcept
{
	return _view_matrix;
}

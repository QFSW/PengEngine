#include "camera.h"

#include <numbers>

#include <core/logger.h>
#include <rendering/window_subsystem.h>
#include <utils/utils.h>

IMPLEMENT_ENTITY(entities::Camera);

using namespace entities;
using namespace rendering;
using namespace math;

peng::weak_ptr<Camera> Camera::_current;

Camera::Camera()
	: Camera("Camera")
{ }

Camera::Camera(const std::string& name)
	: Camera(utils::copy(name))
{ }

Camera::Camera(std::string&& name)
	: Entity(std::move(name), TickGroup::pre_render)
	, _fov(70)
	, _ortho_size(20)
	, _near_clip(0.01f)
	, _far_clip(1000.0f)
	, _projection(Projection::perspective)
	, _view_matrix(Matrix4x4f::identity())
{ }

const peng::weak_ptr<Camera>& Camera::current()
{
	return _current;
}

void Camera::post_create()
{
	Entity::post_create();

	if (_current.valid())
	{
		Logger::warning("Camera entity created when a valid camera already exists");
	}

	_current = weak_this();
	check(_current);
}

void Camera::tick(float delta_time)
{
	Entity::tick(delta_time);

	Matrix4x4f transform_inv = transform_matrix_inv();
	if (_projection == Projection::perspective)
	{
		// Perform z_reverse on perspective transform
		transform_inv = transform_inv.scaled(Vector3f(1, 1, -1));
	}

	_view_matrix = calc_projection_matrix() * transform_inv;
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
		Logger::warning(
			"Camera far-clip (%f) is not grater than near-clip (%f) - unexpected results may occur",
			_far_clip, _near_clip
		);
	}
}

Matrix4x4f Camera::calc_projection_matrix()
{
	const float aspect_ratio = WindowSubsystem::get().aspect_ratio();

	if (_projection == Projection::orthographic)
	{
		Transform ortho_transform;
		ortho_transform.position = Vector3f(0, 0, _near_clip);
		ortho_transform.scale = Vector3f(_ortho_size * aspect_ratio, _ortho_size, _far_clip - _near_clip);

		Vector3f& scale = _local_transform.scale;
		if (scale.x * scale.y * scale.z == 0.0f)
		{
			Logger::warning(
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

	Logger::error("Unsupported projection mode %d", _projection);
	return Matrix4x4f::identity();
}

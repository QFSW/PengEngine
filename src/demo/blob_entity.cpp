#include "blob_entity.h"

#include <core/peng_engine.h>

using namespace math;

BlobEntity::BlobEntity(
	const peng::shared_ref<rendering::Mesh>& mesh,
	const peng::shared_ref<rendering::Material>& material,
	const Vector2f& pos_px,
	float radius_px
)
	: Entity(true)
	, _mesh(mesh)
	, _material(material)
	, _position(pos_px, 0)
	, _scale(radius_px, radius_px, radius_px)
{
	const Vector2f resolution = PengEngine::get().resolution();

	_view_matrix =
		Matrix4x4f::identity()
		.scaled(Vector3f(1 / resolution.x, 1 / resolution.y, 1 / resolution.max()));
}

void BlobEntity::tick(double delta_time)
{
	_angle += static_cast<float>(delta_time);

	const Vector3f rotation = Vector3f(0.5, 1, 0) * _angle;

	const Matrix4x4f transform =
		Matrix4x4f::identity()
		.rotated(rotation)
		.scaled(_scale)
		.translated(_position);

	_material->set_parameter("transform",  _view_matrix * transform);

	_material->use();
	_mesh->render();
}
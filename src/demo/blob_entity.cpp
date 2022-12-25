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
{
	const Vector2f resolution = PengEngine::get().resolution();

	_position = Vector3f(pos_px / (resolution / 2.0f), 0.5f);
	_scale = Vector3f(radius_px / resolution.x, radius_px / resolution.y, radius_px / resolution.x);
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

	_material->set_parameter("transform", transform);

	_material->use();
	_mesh->render();
}
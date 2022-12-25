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
	const Vector2f pos = pos_px / (resolution / 2.0f);
	const Vector2f scale = Vector2f(radius_px / resolution.x, radius_px / resolution.y);

	const Matrix4x4f transform =
		Matrix4x4f::identity()
		.scaled(Vector3f(scale, 1))
		.translated(Vector3f(pos, 0));

	_material->set_parameter("transform", transform);
}

void BlobEntity::tick(double)
{
	_material->use();
	_mesh->render();
}
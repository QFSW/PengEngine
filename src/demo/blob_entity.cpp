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
	, _pos(pos_px)
{
	const Vector2f resolution = PengEngine::get().resolution();
	_pos = pos_px / (resolution / 2.0f);
	_scale = Vector2f(radius_px / resolution.x, radius_px / resolution.y);

	_material->set_parameter("pos", Vector3f(_pos, 0));
	_material->set_parameter("scale", Vector3f(_scale, 1));
}

void BlobEntity::tick(double)
{
	_material->use();
	_mesh->render();
}
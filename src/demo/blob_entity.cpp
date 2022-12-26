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
	, _transform(Vector3f(pos_px, 0), Vector3f::one() * radius_px, Vector3f::zero())
{
	const Vector2f resolution = PengEngine::get().resolution();

	_view_matrix =
		Matrix4x4f::identity()
		.scaled(Vector3f(1 / resolution.x, 1 / resolution.y, 1 / resolution.max()));
}

void BlobEntity::tick(double delta_time)
{
	_transform.rotation += Vector3f(0.5, 1, 0) * static_cast<float>(delta_time);
	_material->set_parameter("transform",  _view_matrix * _transform.to_matrix());

	_material->use();
	_mesh->render();
}
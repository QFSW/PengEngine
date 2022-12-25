#include "blob_entity.h"

BlobEntity::BlobEntity(
	const peng::shared_ref<rendering::Mesh>& mesh,
	const peng::shared_ref<rendering::Material>& material,
	int32_t pos_x,
	int32_t pos_y,
	int32_t radius
)
	: Entity(true)
	, _mesh(mesh)
	, _material(material)
	, _pos_x(pos_x)
	, _pos_y(pos_y)
	, _radius(radius)
{ }

void BlobEntity::tick(double)
{
	_material->use();
	_mesh->render();
}
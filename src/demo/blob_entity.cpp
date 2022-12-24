#include "blob_entity.h"

BlobEntity::BlobEntity(
	const peng::shared_ref<rendering::Shader>& shader,
	const peng::shared_ref<rendering::Mesh>& mesh,
	int32_t pos_x,
	int32_t pos_y,
	int32_t radius
)
	: Entity(true)
	, _shader(shader)
	, _mesh(mesh)
	, _pos_x(pos_x)
	, _pos_y(pos_y)
	, _radius(radius)
{ }

void BlobEntity::tick(double)
{
	_shader->use();
	_mesh->render();
}
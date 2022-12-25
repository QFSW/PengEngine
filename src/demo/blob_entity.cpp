#include "blob_entity.h"

BlobEntity::BlobEntity(
	const peng::shared_ref<rendering::Mesh>& mesh,
	const peng::shared_ref<rendering::Material>& material,
	const math::Vector2f& pos,
	const math::Vector2f& scale
)
	: Entity(true)
	, _mesh(mesh)
	, _material(material)
	, _pos(pos)
	, _scale(scale)
{
	_material->set_parameter("pos", math::Vector3f(_pos, 0));
	_material->set_parameter("scale", math::Vector3f(_scale, 1));
}

void BlobEntity::tick(double)
{
	_material->use();
	_mesh->render();
}
#include "blob_entity.h"

#include <entities/camera.h>

using namespace math;
using namespace entities;

BlobEntity::BlobEntity(
	const peng::shared_ref<const rendering::Mesh>& mesh,
	const peng::shared_ref<rendering::Material>& material,
	const Vector2f& pos
)
	: Entity(true)
	, _mesh(mesh)
	, _material(material)
	, _age(static_cast<float>(rand()) / static_cast<float>((RAND_MAX)))
	, _transform(Vector3f(pos, pos.y), Vector3f::one(), Vector3f::zero())
{ }

void BlobEntity::tick(double delta_time)
{
	_age += static_cast<float>(delta_time);

	const Matrix4x4f view_matrix = Camera::current() ? Camera::current()->view_matrix() : Matrix4x4f::identity();
	_material->set_parameter("transform", view_matrix * _transform.to_matrix());
	_material->set_parameter("time", _age);

	_material->use();
	_mesh->render();
}
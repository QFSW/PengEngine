#pragma once

#include <core/entity.h>
#include <memory/shared_ref.h>
#include <rendering/material.h>
#include <rendering/mesh.h>
#include <math/transform.h>

class BlobEntity : public Entity
{
public:
	BlobEntity(
		const peng::shared_ref<const rendering::Mesh>& mesh,
		const peng::shared_ref<rendering::Material>& material,
		const math::Vector2f& pos
	);

	virtual void tick(double delta_time) override;

private:
	peng::shared_ref<const rendering::Mesh> _mesh;
	peng::shared_ref<rendering::Material> _material;

	float _age;
	math::Transform _transform;
};
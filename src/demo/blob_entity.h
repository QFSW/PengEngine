#pragma once

#include <core/entity.h>
#include <memory/shared_ref.h>
#include <rendering/material.h>
#include <rendering/mesh.h>

class BlobEntity : public Entity
{
public:
	BlobEntity(
		const peng::shared_ref<rendering::Mesh>& mesh,
		const peng::shared_ref<rendering::Material>& material,
		const math::Vector2f& pos_px,
		float radius_px
	);

	virtual void tick(double delta_time) override;

private:
	peng::shared_ref<rendering::Mesh> _mesh;
	peng::shared_ref<rendering::Material> _material;

	float _angle = 0;
	math::Vector3f _position;
	math::Vector3f _scale;
};
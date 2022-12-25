#pragma once

#include <vector>
#include <GL/glew.h>

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
		const math::Vector2f& pos,
		const math::Vector2f& scale
	);

	virtual void tick(double delta_time) override;

private:
	peng::shared_ref<rendering::Mesh> _mesh;
	peng::shared_ref<rendering::Material> _material;

	math::Vector2f _pos;
	math::Vector2f _scale;
};
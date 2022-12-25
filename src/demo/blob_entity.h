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
		int32_t pos_x,
		int32_t pos_y,
		int32_t radius
	);

	virtual void tick(double delta_time) override;

private:
	peng::shared_ref<rendering::Mesh> _mesh;
	peng::shared_ref<rendering::Material> _material;

	int32_t _pos_x;
	int32_t _pos_y;
	int32_t _radius;
};
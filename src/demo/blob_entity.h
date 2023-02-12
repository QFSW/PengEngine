#pragma once

#include <core/entity.h>
#include <memory/shared_ref.h>
#include <rendering/material.h>
#include <rendering/mesh.h>
#include <components/mesh_renderer.h>

class BlobEntity : public Entity
{
	DECLARE_ENTITY(BlobEntity);

public:
	BlobEntity(
		const peng::shared_ref<const rendering::Mesh>& mesh,
		const peng::shared_ref<rendering::Material>& material,
		const math::Vector2f& pos
	);

	void post_create() override;
	void tick(float delta_time) override;

private:
	float _age;
	peng::weak_ptr<components::MeshRenderer> _mesh_renderer;
};
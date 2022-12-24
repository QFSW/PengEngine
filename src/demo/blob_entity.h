#pragma once

#include <vector>
#include <GL/glew.h>

#include <core/entity.h>
#include <memory/shared_ref.h>
#include <rendering/shader.h>
#include <rendering/mesh.h>

class BlobEntity : public Entity
{
public:
	BlobEntity(
		const peng::shared_ref<rendering::Shader>& shader,
		const peng::shared_ref<rendering::Mesh>& mesh,
		int32_t pos_x,
		int32_t pos_y,
		int32_t radius
	);

	virtual void tick(double delta_time) override;

private:
	peng::shared_ref<rendering::Shader> _shader;
	peng::shared_ref<rendering::Mesh> _mesh;

	int32_t _pos_x;
	int32_t _pos_y;
	int32_t _radius;
};
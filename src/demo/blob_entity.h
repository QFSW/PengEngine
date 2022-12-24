#pragma once

#include <vector>
#include <GL/glew.h>

#include <core/entity.h>
#include <rendering/shader.h>
#include <rendering/mesh.h>

class BlobEntity : public Entity
{
public:
	BlobEntity(int32_t pos_x, int32_t pos_y, int32_t radius);

	virtual void tick(double delta_time) override;

private:
	int32_t _pos_x;
	int32_t _pos_y;
	int32_t _radius;

	rendering::Shader _shader;
	rendering::Mesh _mesh;

	static std::vector<float> _vertices;
	static std::vector<GLuint> _indices;

	static std::string _vertShaderSrc;
	static std::string _fragShaderSrc;
};
#include "blob_entity.h"

std::vector<float> BlobEntity::_vertices =
{
	 0.5f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f
};

std::vector<GLuint> BlobEntity::_indices =
{
	0, 1, 3,
	1, 2, 3
};

std::string BlobEntity::_vertShaderSrc =
"\n#version 330 core"
"\nlayout(location = 0) in vec3 aPos;"
"\n"
"\nvoid main()"
"\n{"
"\n	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
"\n}\0";

std::string BlobEntity::_fragShaderSrc =
"\n#version 330 core"
"\nout vec4 FragColor;"
"\n"
"\nvoid main()"
"\n{"
"\n	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
"\n}\0";

BlobEntity::BlobEntity(int32_t pos_x, int32_t pos_y, int32_t radius)
	: Entity(true)
	, _pos_x(pos_x)
	, _pos_y(pos_y)
	, _radius(radius)
	, _shader(_vertShaderSrc, _fragShaderSrc)
	, _mesh(_vertices, _indices)
{ }

void BlobEntity::tick(double)
{
	_shader.use();
	_mesh.render();
}
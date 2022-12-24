#pragma once

#include <vector>
#include <GL/glew.h>

#include <core/entity.h>

class BlobEntity : public Entity
{
public:
	BlobEntity(int32_t pos_x, int32_t pos_y, int32_t radius);

	virtual void tick(double delta_time) override;

private:
	void validate_shader_compile(GLuint shader);
	void validate_shader_link(GLuint shader);

	int32_t _pos_x;
	int32_t _pos_y;
	int32_t _radius;

	float _vertices[9] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	GLuint _vbo;
	GLuint _vao;

	GLuint _vertShader;
	GLuint _fragShader;
	GLuint _shaderProg;

	static const char* _vertShaderSrc;
	static const char* _fragShaderSrc;
};
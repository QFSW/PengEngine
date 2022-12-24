#include "blob_entity.h"

const char* BlobEntity::_vertShaderSrc =
"\n#version 330 core"
"\nlayout(location = 0) in vec3 aPos;"
"\n"
"\nvoid main()"
"\n{"
"\n	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
"\n}\0";

const char* BlobEntity::_fragShaderSrc =
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
{
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
	glGenVertexArrays(1, &_vao);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	_vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vertShader, 1, &_vertShaderSrc, NULL);
	glCompileShader(_vertShader);
	validate_shader_compile(_vertShader);

	_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fragShader, 1, &_fragShaderSrc, NULL);
	glCompileShader(_fragShader);
	validate_shader_compile(_fragShader);

	_shaderProg = glCreateProgram();
	glAttachShader(_shaderProg, _vertShader);
	glAttachShader(_shaderProg, _fragShader);
	glLinkProgram(_shaderProg);
	validate_shader_link(_shaderProg);

	glDeleteShader(_vertShader);
	glDeleteShader(_fragShader);
}

void BlobEntity::tick(double)
{
	// glRasterPos2i(_pos_x - _radius, _pos_y - _radius);
	// glDrawPixels(_radius * 2, _radius * 2, GL_RGB, GL_UNSIGNED_BYTE, _pixel_buffer.data());

	glUseProgram(_shaderProg);
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void BlobEntity::validate_shader_compile(GLuint shader)
{
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("ERROR::SHADER::COMPILATION_FAILED\n%s", infoLog);
	}
}

void BlobEntity::validate_shader_link(GLuint shader)
{
	GLint success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shader, 512, NULL, infoLog);
		printf("ERROR::SHADER::LINK_FAILED\n%s", infoLog);
	}
}
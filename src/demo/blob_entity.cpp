#include "blob_entity.h"

#include <GL/glew.h>

BlobEntity::BlobEntity(int32_t pos_x, int32_t pos_y, int32_t radius)
	: Entity(true)
	, _pos_x(pos_x)
	, _pos_y(pos_y)
	, _radius(radius)
{
	_pixel_buffer.resize(_radius * _radius * 4 * 3, 0xFF);
}

void BlobEntity::tick(double)
{
	// glRasterPos2i(_pos_x - _radius, _pos_y - _radius);
	// glDrawPixels(_radius * 2, _radius * 2, GL_RGB, GL_UNSIGNED_BYTE, _pixel_buffer.data());
}
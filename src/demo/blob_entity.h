#pragma once

#include <vector>

#include <core/entity.h>

class BlobEntity : public Entity
{
public:
	BlobEntity(int32_t pos_x, int32_t pos_y, int32_t radius);

	virtual void tick(double delta_time) override;

private:
	int32_t _pos_x;
	int32_t _pos_y;
	int32_t _radius;

	std::vector<uint8_t> _pixel_buffer;
};
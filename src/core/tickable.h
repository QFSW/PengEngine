#pragma once

class ITickable
{
public:
	virtual void tick(float delta_time) = 0;
};
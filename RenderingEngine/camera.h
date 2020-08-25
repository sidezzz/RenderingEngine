#pragma once
#include "primitives.h"

class Camera
{
public:

private:
	Transform transform;
	Vector2 viewport_size_;
	float fov_;
};
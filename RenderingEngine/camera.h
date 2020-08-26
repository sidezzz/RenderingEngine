#pragma once
#include "primitives.h"

class Camera
{
public:
	Transform GetTransform() const;
	float GetFov() const;
	float GetAspectRatio() const;

//private:
	Transform transform_;
	float aspect_ratio_ = 1.f;
	float fov_ = 90.f;
};
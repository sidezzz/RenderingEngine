#pragma once
#include "primitives.h"

class Camera
{
public:
	Transform GetTransform() const;
	float GetFov() const;
	float GetAspectRatio() const;
	Matrix4x4 GetProjection() const;

//private:
	Transform transform_;
	float aspect_ratio_ = 1.f;
	float fov_ = 90.f;
	float near_plane_ = 1.f;
	float far_plane_ = 800.f;
};
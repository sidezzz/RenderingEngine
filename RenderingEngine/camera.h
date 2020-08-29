#pragma once

#include "scene_object.h"

class Camera : public SceneObject
{
public:
	float GetFov() const;
	float GetAspectRatio() const;
	Matrix4x4 GetPerspectiveProjection() const;
	void SetFov(float fov);
	void SetAspectRatio(float aspect_ratio);

protected:
	float aspect_ratio_ = 1.f;
	float fov_ = 80.f;
	float near_plane_ = 0.01f;
	float far_plane_ = 800.f;
};

#include "camera.h"

Transform Camera::GetTransform() const
{
	return transform_;
}
float Camera::GetFov() const
{
	return fov_;
}
float Camera::GetAspectRatio() const
{
	return aspect_ratio_;
}
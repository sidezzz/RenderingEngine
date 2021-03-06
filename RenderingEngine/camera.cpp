
#include "camera.h"

float Camera::GetFov() const
{
	return fov_;
}
float Camera::GetAspectRatio() const
{
	return aspect_ratio_;
}
Matrix4x4 Camera::GetPerspectiveProjection() const
{
	Matrix4x4 projection;

	auto range_inv = 1.f / (far_plane_ - near_plane_);
	/*projection[1][2] = 1.f / std::tan(0.5f * DegreeToRad(fov_));
	projection[0][1] = projection.m[1][2] / aspect_ratio_;
	projection.m[2][0] = -(near_plane_ + far_plane_) * range_inv;
	projection.m[3][0] = -(near_plane_ * far_plane_) * range_inv;
	projection.m[2][3] = -1.f;
	projection.m[3][3] = 0.f;*/

	projection[1][2] = 1.f / std::tan(0.5f * DegreeToRad(fov_));
	projection[0][1] = projection.m[1][2] / aspect_ratio_;
	projection.m[2][3] = -2.f * (near_plane_ * far_plane_) * range_inv;
	projection.m[2][0] = (near_plane_ + far_plane_) * range_inv;
	projection.m[3][0] = 1.f;
	projection.m[3][3] = 0.f;

	return projection;
}
void Camera::SetFov(float fov)
{
	fov_ = std::clamp(fov, 0.001f, 179.f);
}
void Camera::SetAspectRatio(float aspect_ratio)
{
	aspect_ratio_ = aspect_ratio;
}
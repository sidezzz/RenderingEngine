#include <cmath>

#include "primitives.h"

float Vector3::Size() const
{
	return std::sqrt(SizeSquared());
}
float Vector3::LengthTo(const Vector3& rhs) const
{
	return (*this - rhs).Size();
}
Vector3 Vector3::Normal() const
{
	Vector3 result;
	const auto length = Size();

	if (length != 0.f)
	{
		result.x = x / length;
		result.y = y / length;
		result.z = z / length;
	}
	else
	{
		result.x = result.y = result.z = 0.f;
	}

	return result;
}

Rotator Rotator::Clamp() const
{
	return Rotator(ClampAxis(yaw), ClampAxis(pitch), ClampAxis(roll));
}
float Rotator::ClampAxis(float angle)
{
	const auto ceil_val = (-angle) / 360.f;
	return angle + std::ceil(ceil_val) * 360.f;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rhs)
{
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				result.m[i][j] += m[i][k] * rhs.m[k][j];
			}
		}
	}
	*this = result;
	return *this;
}

Matrix4x4 operator*(Matrix4x4 a, const Matrix4x4& b) 
{ 
	return a *= b; 
}

Matrix4x4 Transform::ToMatrix() const
{
	Matrix4x4 scale_m(
		Vector3(scale.x, 0.f, 0.f),
		Vector3(0.f, scale.y, 0.f),
		Vector3(0.f, 0.f, scale.z),
		Vector3(0.f, 0.f, 0.f)
	);
	scale_m.m[3][3] = 1.f;

	Matrix4x4 trans_m;
	trans_m.m[0][0] = 1.f; trans_m.m[0][3] = translation.x;
	trans_m.m[1][1] = 1.f; trans_m.m[1][3] = translation.y;
	trans_m.m[2][2] = 1.f; trans_m.m[2][3] = translation.z;
	trans_m.m[3][3] = 1.f;

	Matrix4x4 rot_x_m(
		Vector3(scale.x, 0.f, 0.f),
		Vector3(0.f, scale.y, 0.f),
		Vector3(0.f, 0.f, scale.z),
		Vector3(0.f, 0.f, 0.f)
	);

	return scale_m * trans_m;
}
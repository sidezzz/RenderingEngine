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

float Matrix4x4::Determinant() const
{
	return	m[0][0] * (
		m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
		m[2][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) +
		m[3][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2])
		) -
		m[1][0] * (
			m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) -
			m[2][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2])
			) +
		m[2][0] * (
			m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) -
			m[1][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2]) +
			m[3][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			) -
		m[3][0] * (
			m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) -
			m[1][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2]) +
			m[2][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
		);
}
Matrix4x4 Matrix4x4::Inverse() const
{
	Matrix4x4 result;
	float det[4];
	Matrix4x4 tmp;

	tmp.m[0][0] = m[2][2] * m[3][3] - m[2][3] * m[3][2];
	tmp.m[0][1] = m[1][2] * m[3][3] - m[1][3] * m[3][2];
	tmp.m[0][2] = m[1][2] * m[2][3] - m[1][3] * m[2][2];

	tmp.m[1][0] = m[2][2] * m[3][3] - m[2][3] * m[3][2];
	tmp.m[1][1] = m[0][2] * m[3][3] - m[0][3] * m[3][2];
	tmp.m[1][2] = m[0][2] * m[2][3] - m[0][3] * m[2][2];

	tmp.m[2][0] = m[1][2] * m[3][3] - m[1][3] * m[3][2];
	tmp.m[2][1] = m[0][2] * m[3][3] - m[0][3] * m[3][2];
	tmp.m[2][2] = m[0][2] * m[1][3] - m[0][3] * m[1][2];

	tmp.m[3][0] = m[1][2] * m[2][3] - m[1][3] * m[2][2];
	tmp.m[3][1] = m[0][2] * m[2][3] - m[0][3] * m[2][2];
	tmp.m[3][2] = m[0][2] * m[1][3] - m[0][3] * m[1][2];

	det[0] = m[1][1] * tmp.m[0][0] - m[2][1] * tmp.m[0][1] + m[3][1] * tmp.m[0][2];
	det[1] = m[0][1] * tmp.m[1][0] - m[2][1] * tmp.m[1][1] + m[3][1] * tmp.m[1][2];
	det[2] = m[0][1] * tmp.m[2][0] - m[1][1] * tmp.m[2][1] + m[3][1] * tmp.m[2][2];
	det[3] = m[0][1] * tmp.m[3][0] - m[1][1] * tmp.m[3][1] + m[2][1] * tmp.m[3][2];

	float determinant = m[0][0] * det[0] - m[1][0] * det[1] + m[2][0] * det[2] - m[3][0] * det[3];
	const float	rdet = 1.0f / determinant;

	result.m[0][0] = rdet * det[0];
	result.m[0][1] = -rdet * det[1];
	result.m[0][2] = rdet * det[2];
	result.m[0][3] = -rdet * det[3];
	result.m[1][0] = -rdet * (m[1][0] * tmp.m[0][0] - m[2][0] * tmp.m[0][1] + m[3][0] * tmp.m[0][2]);
	result.m[1][1] = rdet * (m[0][0] * tmp.m[1][0] - m[2][0] * tmp.m[1][1] + m[3][0] * tmp.m[1][2]);
	result.m[1][2] = -rdet * (m[0][0] * tmp.m[2][0] - m[1][0] * tmp.m[2][1] + m[3][0] * tmp.m[2][2]);
	result.m[1][3] = rdet * (m[0][0] * tmp.m[3][0] - m[1][0] * tmp.m[3][1] + m[2][0] * tmp.m[3][2]);
	result.m[2][0] = rdet * (
		m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
		m[2][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) +
		m[3][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1])
		);
	result.m[2][1] = -rdet * (
		m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
		m[2][0] * (m[0][1] * m[3][3] - m[0][3] * m[3][1]) +
		m[3][0] * (m[0][1] * m[2][3] - m[0][3] * m[2][1])
		);
	result.m[2][2] = rdet * (
		m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) -
		m[1][0] * (m[0][1] * m[3][3] - m[0][3] * m[3][1]) +
		m[3][0] * (m[0][1] * m[1][3] - m[0][3] * m[1][1])
		);
	result.m[2][3] = -rdet * (
		m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) -
		m[1][0] * (m[0][1] * m[2][3] - m[0][3] * m[2][1]) +
		m[2][0] * (m[0][1] * m[1][3] - m[0][3] * m[1][1])
		);
	result.m[3][0] = -rdet * (
		m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
		m[2][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) +
		m[3][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
		);
	result.m[3][1] = rdet * (
		m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
		m[2][0] * (m[0][1] * m[3][2] - m[0][2] * m[3][1]) +
		m[3][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1])
		);
	result.m[3][2] = -rdet * (
		m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) -
		m[1][0] * (m[0][1] * m[3][2] - m[0][2] * m[3][1]) +
		m[3][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1])
		);
	result.m[3][3] = rdet * (
		m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
		m[1][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1]) +
		m[2][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1])
		);
	return result;
}
Matrix4x4 Matrix4x4::Transpose() const
{
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result.m[i][j] = m[j][i];
		}
	}
	return result;
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
	Matrix4x4 scale_trans_m(
		Vector3(scale.x, 0.f, 0.f),
		Vector3(0.f, scale.y, 0.f),
		Vector3(0.f, 0.f, scale.z),
		Vector3(0.f, 0.f, 0.f)
	);

	scale_trans_m.m[0][3] = translation.x;
	scale_trans_m.m[1][3] = translation.y;
	scale_trans_m.m[2][3] = translation.z;

	auto rad_rotator = DegreeToRad(rotation);
	auto cw = std::cos(rad_rotator.yaw);
	auto sw = std::sin(rad_rotator.yaw);
	auto cv = std::cos(rad_rotator.pitch);
	auto sv = std::sin(rad_rotator.pitch);
	auto cu = std::cos(rad_rotator.roll);
	auto su = std::sin(rad_rotator.roll);

	Matrix4x4 rot_m(
		Vector3(cv * cw, su * sv * cw - cu * sw, su * sw + cu * sv * cw),
		Vector3(cv * sw, cu * cw + su * sv * sw, cu * sv * sw - su * cw),
		Vector3(-sv, su * cv, cu * cv),
		Vector3(0.f, 0.f, 0.f)
	);

	return scale_trans_m * rot_m.Transpose();
}
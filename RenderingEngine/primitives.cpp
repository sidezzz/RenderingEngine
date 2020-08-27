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

Vector3 Rotator::RotateVector(const Vector3& vec) const
{
	auto matrix = RotationMatrix();

	Vector3 result;
	result.x = vec.x * matrix[0][0] + vec.y * matrix[0][1] + vec.z * matrix[0][2];
	result.y = vec.x * matrix[1][0] + vec.y * matrix[1][1] + vec.z * matrix[1][2];
	result.z = vec.x * matrix[2][0] + vec.y * matrix[2][1] + vec.z * matrix[2][2];

	return result;
}
Matrix4x4 Rotator::RotationMatrix() const
{
	auto rad_rotator = DegreeToRad(*this);
	auto cw = std::cos(rad_rotator.yaw);
	auto sw = std::sin(rad_rotator.yaw);
	auto cv = std::cos(rad_rotator.pitch);
	auto sv = std::sin(rad_rotator.pitch);
	auto cu = std::cos(rad_rotator.roll);
	auto su = std::sin(rad_rotator.roll);

	return Matrix4x4(
		Vector3(cv * cw, su * sv * cw - cu * sw, su * sw + cu * sv * cw),
		Vector3(cv * sw, cu * cw + su * sv * sw, cu * sv * sw - su * cw),
		Vector3(-sv, su * cv, cu * cv),
		Vector3(0.f, 0.f, 0.f)
	);
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

	tmp[0][0] = m[2][2] * m[3][3] - m[2][3] * m[3][2];
	tmp[0][1] = m[1][2] * m[3][3] - m[1][3] * m[3][2];
	tmp[0][2] = m[1][2] * m[2][3] - m[1][3] * m[2][2];

	tmp[1][0] = m[2][2] * m[3][3] - m[2][3] * m[3][2];
	tmp[1][1] = m[0][2] * m[3][3] - m[0][3] * m[3][2];
	tmp[1][2] = m[0][2] * m[2][3] - m[0][3] * m[2][2];

	tmp[2][0] = m[1][2] * m[3][3] - m[1][3] * m[3][2];
	tmp[2][1] = m[0][2] * m[3][3] - m[0][3] * m[3][2];
	tmp[2][2] = m[0][2] * m[1][3] - m[0][3] * m[1][2];

	tmp[3][0] = m[1][2] * m[2][3] - m[1][3] * m[2][2];
	tmp[3][1] = m[0][2] * m[2][3] - m[0][3] * m[2][2];
	tmp[3][2] = m[0][2] * m[1][3] - m[0][3] * m[1][2];

	det[0] = m[1][1] * tmp[0][0] - m[2][1] * tmp[0][1] + m[3][1] * tmp[0][2];
	det[1] = m[0][1] * tmp[1][0] - m[2][1] * tmp[1][1] + m[3][1] * tmp[1][2];
	det[2] = m[0][1] * tmp[2][0] - m[1][1] * tmp[2][1] + m[3][1] * tmp[2][2];
	det[3] = m[0][1] * tmp[3][0] - m[1][1] * tmp[3][1] + m[2][1] * tmp[3][2];

	float determinant = m[0][0] * det[0] - m[1][0] * det[1] + m[2][0] * det[2] - m[3][0] * det[3];
	const float	rdet = 1.0f / determinant;

	result[0][0] = rdet * det[0];
	result[0][1] = -rdet * det[1];
	result[0][2] = rdet * det[2];
	result[0][3] = -rdet * det[3];
	result[1][0] = -rdet * (m[1][0] * tmp[0][0] - m[2][0] * tmp[0][1] + m[3][0] * tmp[0][2]);
	result[1][1] = rdet * (m[0][0] * tmp[1][0] - m[2][0] * tmp[1][1] + m[3][0] * tmp[1][2]);
	result[1][2] = -rdet * (m[0][0] * tmp[2][0] - m[1][0] * tmp[2][1] + m[3][0] * tmp[2][2]);
	result[1][3] = rdet * (m[0][0] * tmp[3][0] - m[1][0] * tmp[3][1] + m[2][0] * tmp[3][2]);
	result[2][0] = rdet * (
		m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
		m[2][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) +
		m[3][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1])
		);
	result[2][1] = -rdet * (
		m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
		m[2][0] * (m[0][1] * m[3][3] - m[0][3] * m[3][1]) +
		m[3][0] * (m[0][1] * m[2][3] - m[0][3] * m[2][1])
		);
	result[2][2] = rdet * (
		m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) -
		m[1][0] * (m[0][1] * m[3][3] - m[0][3] * m[3][1]) +
		m[3][0] * (m[0][1] * m[1][3] - m[0][3] * m[1][1])
		);
	result[2][3] = -rdet * (
		m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) -
		m[1][0] * (m[0][1] * m[2][3] - m[0][3] * m[2][1]) +
		m[2][0] * (m[0][1] * m[1][3] - m[0][3] * m[1][1])
		);
	result[3][0] = -rdet * (
		m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
		m[2][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) +
		m[3][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
		);
	result[3][1] = rdet * (
		m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
		m[2][0] * (m[0][1] * m[3][2] - m[0][2] * m[3][1]) +
		m[3][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1])
		);
	result[3][2] = -rdet * (
		m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) -
		m[1][0] * (m[0][1] * m[3][2] - m[0][2] * m[3][1]) +
		m[3][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1])
		);
	result[3][3] = rdet * (
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
			result[i][j] = m[j][i];
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
				result[i][j] += m[i][k] * rhs[k][j];
			}
		}
	}
	*this = result;
	return *this;
}
const float* Matrix4x4::operator[](int index) const
{
	return m[index];
}
float* Matrix4x4::operator[](int index)
{
	return m[index];
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

	scale_trans_m[0][3] = translation.x;
	scale_trans_m[1][3] = translation.y;
	scale_trans_m[2][3] = translation.z;

	return scale_trans_m * rotation.RotationMatrix();
}
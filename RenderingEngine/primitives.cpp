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
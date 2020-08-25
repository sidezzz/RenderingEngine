#pragma once
#include <cmath>

constexpr static auto M_PI = 3.14159265359;

template<class T>
T DegreeToRad(const T& angle)
{
	return angle * (M_PI / 180.0);
}
template<class T>
constexpr T RadToDegree(const T& angle)
{
	return angle / (M_PI / 180.0);
}

struct Vector2
{
	union
	{
		struct
		{
			float x;
			float y;
		};
		float raw[2] = { 0 };
	};
};

struct Vector3
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		float raw[3] = { 0 };
	};

	constexpr Vector3() = default;
	constexpr Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	float Size() const;
	float LengthTo(const Vector3& rhs) const;
	Vector3 Normal() const;

	constexpr float SizeSquared() const
	{
		return x * x + y * y + z * z;
	}
	constexpr float Dot(const Vector3& rhs) const
	{
		return (x * rhs.x + y * rhs.y + z * rhs.z);
	}

	constexpr bool operator==(const Vector3& rhs) const
	{
		return (rhs.x == x) && (rhs.y == y) && (rhs.z == z);
	}
	constexpr bool operator!=(const Vector3& rhs) const
	{
		return !(*this == rhs);
	}
	constexpr bool operator>(const Vector3& rhs) const
	{
		return (x + y + z) > (rhs.x + rhs.y + rhs.z);
	}
	constexpr bool operator<(const Vector3& rhs) const
	{
		return (x + y + z) < (rhs.x + rhs.y + rhs.z);
	}

	constexpr Vector3& operator+=(const Vector3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	constexpr Vector3& operator-=(const Vector3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	constexpr Vector3& operator*=(const Vector3& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}
	constexpr Vector3& operator/=(const Vector3& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}
	constexpr Vector3& operator*=(float scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		return *this;
	}
	constexpr Vector3& operator/=(float scale)
	{
		x /= scale;
		y /= scale;
		z /= scale;
		return *this;
	}
};

constexpr Vector3 operator+(Vector3 a, const Vector3& b) { return a += b; }
constexpr Vector3 operator-(Vector3 a, const Vector3& b) { return a -= b; }
constexpr Vector3 operator*(Vector3 a, const Vector3& b) { return a *= b; }
constexpr Vector3 operator/(Vector3 a, const Vector3& b) { return a /= b; }
constexpr Vector3 operator*(Vector3 a, float b) { return a *= b; }
constexpr Vector3 operator/(Vector3 a, float b) { return a /= b; }

// angles stored as degrees [0, 360)
struct Rotator
{
	union
	{
		struct
		{
			float yaw;
			float pitch;
			float roll;
		};
		float raw[3] = { 0 };
	};

	constexpr Rotator() = default;
	constexpr Rotator(float _yaw, float _pitch, float _roll) : yaw(_yaw), pitch(_pitch), roll(_roll) {}

	Rotator Clamp() const;
	static float ClampAxis(float angle);

	constexpr bool operator==(const Rotator& rhs) const
	{
		return (rhs.yaw == yaw) && (rhs.pitch == pitch) && (rhs.roll == roll);
	}
	constexpr bool operator!=(const Rotator& rhs) const
	{
		return !(*this == rhs);
	}

	constexpr Rotator& operator+=(const Rotator& rhs)
	{
		yaw += rhs.yaw;
		pitch += rhs.pitch;
		roll += rhs.roll;
		return *this;
	}
	constexpr Rotator& operator-=(const Rotator& rhs)
	{
		yaw += rhs.yaw;
		pitch += rhs.pitch;
		roll += rhs.roll;
		return *this;
	}
	constexpr Rotator& operator*=(float scale)
	{
		yaw *= scale;
		pitch *= scale;
		roll *= scale;
		return *this;
	}
	constexpr Rotator& operator/=(float scale)
	{
		yaw /= scale;
		pitch /= scale;
		roll /= scale;
		return *this;
	}
};

constexpr Rotator operator+(Rotator a, const Rotator& b) { return a += b; }
constexpr Rotator operator-(Rotator a, const Rotator& b) { return a -= b; }
constexpr Rotator operator*(Rotator a, float b) { return a *= b; }
constexpr Rotator operator/(Rotator a, float b) { return a /= b; }

struct Matrix4x4
{
	union
	{
		float m[4][4];
		float raw[16] = { 0 };
	};

	constexpr Matrix4x4() = default;
	constexpr Matrix4x4(const Vector3& x, const Vector3& y, const Vector3& z, const Vector3& w)
	{
		m[0][0] = x.x; m[0][1] = x.y; m[0][2] = x.z; m[3][3] = 0.f;
		m[1][0] = y.x; m[1][1] = y.y; m[1][2] = y.z; m[3][3] = 0.f;
		m[2][0] = z.x; m[2][1] = z.y; m[2][2] = z.z; m[3][3] = 0.f;
		m[3][0] = w.x; m[3][1] = w.y; m[3][2] = w.z; m[3][3] = 1.f;
	}

	Matrix4x4 Inverse() const;

	Matrix4x4& operator*=(const Matrix4x4& rhs);
};

Matrix4x4 operator*(Matrix4x4 a, const Matrix4x4& b);

struct Transform
{
	Vector3 translation;
	Rotator rotation;
	Vector3 scale = Vector3(1.f, 1.f, 1.f);

	constexpr Transform() = default;
	constexpr Transform(const Vector3& _translation, const Rotator& _rotation, const Vector3& _scale = Vector3(1.f, 1.f, 1.f))
		: translation(_translation), rotation(_rotation), scale(_scale) {}

	Matrix4x4 ToMatrix() const;
};

struct Color
{
	union
	{
		struct
		{
			float r;
			float g;
			float b;
		};
		float raw[3] = { 0 };
	};

	constexpr Color() = default;
};

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Color color;
	Vector2 uv;
};
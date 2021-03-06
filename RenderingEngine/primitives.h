#pragma once
#include <cmath>
#include <cstdint>
#include <algorithm>

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

	float Determinant() const;
	Matrix4x4 Inverse() const;
	Matrix4x4 Transpose() const;

	Matrix4x4& operator*=(const Matrix4x4& rhs);
	const float* operator[](int index) const;
	float* operator[](int index);
};

Matrix4x4 operator*(Matrix4x4 a, const Matrix4x4& b);

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

	Vector3 RotateVector(const Vector3& vector) const;
	Matrix4x4 RotationMatrix() const;
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

struct ColorInt;
struct ColorFloat;

struct ColorFloat
{
	union
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
		float raw[4] = { 0 };
	};

	constexpr ColorFloat() = default;
	constexpr ColorFloat(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
	constexpr ColorFloat(const ColorInt& color);
};

struct ColorInt
{
	union
	{
		struct
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};
		uint32_t value;
		uint8_t raw[4] = { 0 };
	};

	constexpr ColorInt() = default;
	constexpr ColorInt(uint32_t _raw)
	{
		r = _raw >> 24;
		g = _raw >> 16;
		b = _raw >> 8;
		a = _raw;
	}
	constexpr ColorInt(const ColorFloat& color);
};

constexpr ColorFloat::ColorFloat(const ColorInt& color)
{
	r = std::clamp(static_cast<float>(color.r) / 255.f, 0.f, 1.f);
	g = std::clamp(static_cast<float>(color.g) / 255.f, 0.f, 1.f);
	b = std::clamp(static_cast<float>(color.b) / 255.f, 0.f, 1.f);
	a = std::clamp(static_cast<float>(color.a) / 255.f, 0.f, 1.f);
}

constexpr ColorInt::ColorInt(const ColorFloat& color)
{
	r = static_cast<uint8_t>(std::clamp(color.r * 255.f, 0.f, 255.f));
	g = static_cast<uint8_t>(std::clamp(color.g * 255.f, 0.f, 255.f));
	b = static_cast<uint8_t>(std::clamp(color.b * 255.f, 0.f, 255.f));
	a = static_cast<uint8_t>(std::clamp(color.a * 255.f, 0.f, 255.f));
}
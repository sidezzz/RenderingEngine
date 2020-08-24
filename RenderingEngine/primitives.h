#pragma once

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
};

struct Transform
{
	Vector3 translation;
	Rotator rotation;
	Vector3 scale;
	constexpr Transform() = default;
	constexpr Transform(const Vector3& _translation, const Rotator& _rotation, const Vector3& _scale)
		: translation(_translation), rotation(_rotation), scale(_scale) {}
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
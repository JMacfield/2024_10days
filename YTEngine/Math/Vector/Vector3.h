#pragma once

struct Vector3 {
	float x;
	float y;
	float z;
	Vector3 operator+(const Vector3& rhs) const
	{
		Vector3 result{};
		result.x = x + rhs.x;
		result.y = y + rhs.y;
		result.z = z + rhs.z;
		return result;
	}

	Vector3 operator-(const Vector3& rhs) const
	{
		Vector3 result{};
		result.x = x - rhs.x;
		result.y = y - rhs.y;
		result.z = z - rhs.z;
		return result;
	}

	Vector3 operator*(const Vector3& rhs) const
	{
		Vector3 result{};
		result.x = x * rhs.x;
		result.y = y * rhs.y;
		result.z = z * rhs.z;
		return result;
	}

	Vector3 operator*(float scalar) const
	{
		Vector3 result{};
		result.x = x * scalar;
		result.y = y * scalar;
		result.z = z * scalar;
		return result;
	}

	Vector3 operator/(float scalar) const
	{
		Vector3 result{};
		if (scalar != 0.0f)
		{
			result.x = x / scalar;
			result.y = y / scalar;
			result.z = z / scalar;
		}
		return result;
	}

	Vector3 operator+=(const Vector3& rhs)
	{
		*this = *this + rhs;
		return *this;
	}

	Vector3 operator-=(const Vector3& rhs)
	{
		*this = *this - rhs;
		return *this;
	}

	Vector3 operator*=(const Vector3& rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	Vector3 operator*=(float scalar)
	{
		*this = *this * scalar;
		return *this;
	}

	Vector3 operator/=(float scalar)
	{
		*this = *this / scalar;
		return *this;
	}

	bool operator==(const Vector3& rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	bool operator!=(const Vector3& rhs) const
	{
		return !(*this == rhs);
	}
};
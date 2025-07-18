#pragma once
#include <cmath>
#include <cstdint>

namespace
{
	template<typename T>
	auto Plus2(const T& left, const T& right)
	{
		return T{ left.x + right.x, left.y + right.y };
	}
	template<typename T>
	auto Mul2(const T& left, const T& right)
	{
		return T{ left.x * right.x, left.y * right.y };
	}
	template<typename T, typename Scalar>
	auto Mul2(const T& left, Scalar right)
	{
		return T{ left.x * right, left.y * right };
	}
	template<typename T>
	auto Div2(const T& left, const T& right)
	{
		return T{ left.x / right.x, left.y / right.y };
	}
	template<typename T>
	auto Plus3(const T& left, const T& right)
	{
		return T{ left.x + right.x, left.y + right.y, left.z + right.z };
	}
	template<typename T>
	auto Mul3(const T& left, const T& right)
	{
		return T{ left.x * right.x, left.y * right.y, left.z * right.z };
	}
	template<typename T, typename Scalar>
	auto Mul3(const T& left, const Scalar& right)
	{
		return T{ left.x * right, left.y * right, left.z * right };
	}
	template<typename T>
	auto Div3(const T& left, const T& right)
	{
		return T{ left.x / right.x, left.y / right.y, left.z / right.z };
	}
}

namespace Erm
{
	using int64 = int64_t;
	using int32 = int32_t;
	using float32 = float;

	static constexpr float32 PI = 3.14159265358979323846f;
	static constexpr float32 toRad = PI / 180.f;

	inline float32 cosDegree(float32 degrees)
	{
		return cosf(degrees * toRad);
	}

	inline float32 sinDegree(float32 degrees)
	{
		return sinf(degrees * toRad);
	}

	struct vec2f;
	struct vec2i;
	struct vec3f;
	struct vec3i;

	struct vec2f
	{
		float32 x;
		float32 y;

		vec2f() = default;

		vec2f(float32 x, float32 y);

		vec2f(const vec2i& other);

		float32 Length() const;

		inline vec2f& operator+=(const vec2f& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}
		inline vec2f& operator-=(const vec2f& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}
		inline vec2f& operator*=(const vec2f& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}
		inline vec2f& operator*=(float32 other)
		{
			x *= other;
			y *= other;
			return *this;
		}
		inline vec2f& operator/=(const vec2f& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}
	};

	struct vec2i
	{
		int32 x;
		int32 y;

		inline vec2i& operator+=(const vec2i& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}
		inline vec2i& operator-=(const vec2i& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}
		inline vec2i& operator*=(const vec2i& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}
		inline vec2i& operator/=(const vec2i& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}
	};

	struct vec3f
	{
		float32 x;
		float32 y;
		float32 z;

		inline vec2f xy() const
		{
			return { x, y };
		}

		float32 Length() const
		{
			return sqrtf(x * x + y * y + z * z);
		}

		inline vec3f& operator+=(const vec3f& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		inline vec3f& operator-=(const vec3f& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		inline vec3f& operator*=(const vec3f& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}
		inline vec3f& operator/=(const vec3f& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}
	};

	struct vec3i
	{
		int32 x;
		int32 y;
		int32 z;

		inline vec3i& operator+=(const vec3i& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		inline vec3i& operator-=(const vec3i& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		inline vec3i& operator*=(const vec3i& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}
		inline vec3i& operator/=(const vec3i& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}
	};
	
	inline vec2i operator-(const vec2i& left)
	{
		return { -left.x , -left.y };
	}

	inline vec2f operator-(const vec2f& left)
	{
		return { -left.x , -left.y };
	}

	inline vec2f operator+(const vec2f& left, const vec2f& right)
	{
		return Plus2(left, right);
	}

	inline vec2f operator-(const vec2f& left, const vec2f& right)
	{
		return Plus2(left, -right);
	}

	inline vec2f operator*(const vec2f& left, const vec2f& right)
	{
		return Mul2(left, right);
	}

	inline vec2f operator*(float32 left, const vec2f& right)
	{
		return Mul2(right, left);
	}

	inline vec2f operator*(const vec2f& left, float32 right)
	{
		return Mul2(left, right);
	}

	inline vec2f operator/(float32 left, const vec2f& right)
	{
		left = 1.f / left;
		return Mul2(right, left);
	}

	inline vec2f operator/(const vec2f& left, float32 right)
	{
		right = 1.f / right;
		return Mul2(left, right);
	}

	inline vec2f operator/(const vec2f& left, const vec2f& right)
	{
		return Div2(left, right);
	}

	inline vec3f operator-(const vec3f& left)
	{
		return { -left.x, -left.y, -left.z };
	}

	inline vec3f operator+(const vec3f& left, const vec3f& right)
	{
		return Plus3(left, right);
	}

	inline vec3f operator-(const vec3f& left, const vec3f& right)
	{
		return Plus3(left, -right);
	}

	inline vec3f operator*(const vec3f& left, const vec3f& right)
	{
		return Mul3(left, right);
	}

	inline vec3f operator*(const vec3f& left, float32 right)
	{
		return Mul3(left, right);
	}

	inline vec3f operator*(float32 left, const vec3f& right)
	{
		return Mul3(right, left);
	}

	inline vec3f operator/(const vec3f& left, const vec3f& right)
	{
		return Div3(left, right);
	}

	inline vec3f operator/(const vec3f& left, float32 right)
	{
		right = 1.f / right;
		return Mul3(left, right);
	}

	inline vec3f operator/(float32 left, const vec3f& right)
	{
		left = 1.f / left;
		return Mul3(right, left);
	}

	template<typename T>
	auto distance(const T& left, const T& right)
	{
		return (right - left).Length();
	}

	template<typename T>
	auto normalized(const T& left)
	{
		return left / left.Length();
	}
}

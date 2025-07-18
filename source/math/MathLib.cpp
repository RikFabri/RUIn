#include "MathLib.h"

Erm::vec2f::vec2f(float32 x, float32 y)
	: x(x)
	, y(y)
{
}

Erm::vec2f::vec2f(const vec2i& other)
	: x(float(other.x))
	, y(float(other.y))
{
}

Erm::float32 Erm::vec2f::Length() const
{
	return sqrtf(x * x + y * y);
}

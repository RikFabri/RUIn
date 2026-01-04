#pragma once
#include "../include/RUIN.h"
#include "MathLib.h"
#include <cfloat>
#include <cmath>

namespace RUIN
{
	struct RenderArea
	{
		float x = 0.f, y = 0.f;
		float w = 0.f, h = 0.f;

		bool operator==(const RenderArea& other)
		{
			return x == other.x &&
					y == other.y &&
					w == other.w &&
					h == other.h;
		}

		bool ContainsPoint(int px, int py) const
		{
			return x <= px && y <= py && x + w >= px && y + h >= py;
		}

		bool IsEmpty() const
		{
			return 
				abs(w) < FLT_EPSILON ||
				abs(h) < FLT_EPSILON;
		}

		RUIN_Rectangle GetRect() const
		{
			return {
				uint32_t(x),
				uint32_t(y),
				uint32_t(w),
				uint32_t(h)
			};
		}

		Erm::vec2f GetDimensions() const
		{
			return { w, h };
		}

		Erm::vec2f GetPosition() const
		{
			return { x, y };
		}

		void OffsetBy(const Erm::vec2f& offset)
		{
			x += offset.x;
			y += offset.y;
		}
	};
}

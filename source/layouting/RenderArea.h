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

		bool Overlaps(const RenderArea& other) const
		{
			return
				other.x + other.w >= x &&
				other.y + other.h >= y &&
				other.x <= x + w &&
				other.y <= y + h;
		}

		bool Contains(const RenderArea& other) const
		{
			const bool p1 = ContainsPoint(int(other.x), int(other.y));
			const bool p2 = ContainsPoint(int(other.x + other.w), int(other.y));
			const bool p3 = ContainsPoint(int(other.x + other.w), int(other.y + other.h));
			const bool p4 = ContainsPoint(int(other.x), int(other.y + other.h));

			return p1 && p2 && p3 && p4;
		}

		bool IntersectsEdge(const RenderArea& other) const
		{
			if (IsEmpty() && other.IsEmpty()) return true;

			const bool p1 = ContainsPoint(int(other.x), int(other.y));
			const bool p2 = ContainsPoint(int(other.x + other.w), int(other.y));
			const bool p3 = ContainsPoint(int(other.x + other.w), int(other.y + other.h));
			const bool p4 = ContainsPoint(int(other.x), int(other.y + other.h));

			const bool allTheSame =
				p1 == p2 &&
				p2 == p3 &&
				p3 == p4;

			return !allTheSame;
		}
	};
}

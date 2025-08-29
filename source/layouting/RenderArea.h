#pragma once
#include "../include/RUIN.h"

namespace RUIN
{
	struct RenderArea
	{
		float x = 0.f, y = 0.f;
		float w = 0.f, h = 0.f;

		bool ContainsPoint(int px, int py) const
		{
			return x <= px && y <= py && x + w >= px && y + h >= py;
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
	};
}

#pragma once
#include "../include/RUIN.h"

namespace RUIN
{
	struct RenderArea
	{
		float x, y;
		float w, h;

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

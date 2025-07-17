#pragma once

struct RenderArea
{
	float x, y;
	float w, h;

	bool ContainsPoint(int px, int py) const
	{
		return x <= px && y <= py && x + w >= px && y + h >= py;
	}
};

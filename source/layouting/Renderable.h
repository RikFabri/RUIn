#pragma once
#include "RenderArea.h"

namespace RUIN
{
	class IRenderable
	{
	public:
		virtual ~IRenderable() = default;

		// Draw your content, constrained to targetArea
		virtual void Render(const RenderArea& targetArea) = 0;

		// Return what area you're using within the available area.
		virtual RenderArea CalculateUsedContentArea(const RenderArea& availableArea) = 0;
	};
}
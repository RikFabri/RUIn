#pragma once
#include "../layouting/LeafNode.h"
#include "../UIManager.h"
#include "tinyxml2.h"
#include <string>
#include <iostream>

namespace RUIN
{
	class Button : public UIContainer
	{
	public:
		Button(tinyxml2::XMLElement* element);
		void Render(const RenderArea& targetArea) override;

	private:

		RenderArea GetAreaForChild(const RenderArea& availableArea, RenderArea& usedArea, RenderContext& ctx) const override;
		bool HandleMouseMoved(int cursorX, int cursorY) override;

		bool hovered = false;
	};

}
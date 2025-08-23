#include "Button.h"
#include "../UIManager.h"
#include "databinding/DataBinding.h"


RUIN::Button::Button(tinyxml2::XMLElement* element)
	: UIContainer(element)
{
}

void RUIN::Button::Render(const RenderArea& targetArea)
{
	UIContainer::Render(targetArea);

	if (hovered)
	{
		UIManager::GetInstance().DrawRectangle(targetArea, { 0, 0, 0, 128 });
		hovered = false;
	}
}

RUIN::RenderArea RUIN::Button::GetAreaForChild(const RenderArea& availableArea, RenderArea& usedArea, RenderContext& ctx) const
{
	RenderArea rc = availableArea;

	return rc;
}

bool RUIN::Button::HandleMouseMoved(int cursorX, int cursorY)
{
	hovered = true;
	return true;
}

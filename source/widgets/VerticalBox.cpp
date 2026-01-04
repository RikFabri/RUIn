#include "VerticalBox.h"

RUIN::VerticalBox::VerticalBox(tinyxml2::XMLElement* e)
	: UIContainer(e)
{
	e->QueryBoolAttribute("isFlexible", &m_Flex);
}

RUIN::RenderArea RUIN::VerticalBox::GetAreaForChild(const RenderArea& availableArea, const RenderArea& usedArea, const RenderContext& ctx) const
{
	RenderArea ra = availableArea;

	if (!m_Flex)
	{
		const float height = availableArea.h / GetNumChildren();
		ra.h = height;
		ra.y += height * ctx.childIndex;
	}
	else
	{
		ra.y = usedArea.h;
		ra.h -= usedArea.h;
	}

	return ra;
}
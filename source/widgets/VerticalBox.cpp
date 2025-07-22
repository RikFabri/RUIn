#include "VerticalBox.h"

RUIN::VerticalBox::VerticalBox(tinyxml2::XMLElement* e)
	: UIContainer(e)
{
	e->QueryBoolAttribute("isFlexible", &m_Flex);
}

RUIN::RenderArea RUIN::VerticalBox::GetAreaForChild(const RenderArea& availableArea, RenderArea& usedArea, RenderContext& ctx) const
{
	RenderArea rc = availableArea;

	if (!m_Flex)
	{
		const float height = availableArea.h / GetNumChildren();
		rc.h = height;
		rc.y += height * ctx.childIndex;
	}
	else
	{
		rc.y = usedArea.h;
	}

	return rc;
}
#include "HorizontalBox.h"

RUIN::HorizontalBox::HorizontalBox(tinyxml2::XMLElement* e)
	: UIContainer(e)
	, m_Flex(false)
{
	e->QueryBoolAttribute("isFlexible", &m_Flex);
}

RUIN::RenderArea RUIN::HorizontalBox::GetAreaForChild(const RenderArea& availableArea, const RenderArea& usedArea, const RenderContext& ctx) const
{
	RenderArea rc = availableArea;

	if (!m_Flex)
	{
		const float width = availableArea.w / GetNumChildren();
		rc.w = width;
		rc.x += width * ctx.childIndex;
	}
	else
	{
		rc.x = usedArea.w;
	}

	return rc;
}
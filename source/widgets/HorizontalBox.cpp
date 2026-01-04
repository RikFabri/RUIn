#include "HorizontalBox.h"

RUIN::HorizontalBox::HorizontalBox(tinyxml2::XMLElement* e)
	: UIContainer(e)
	, m_Flex(false)
{
	e->QueryBoolAttribute("isFlexible", &m_Flex);
}

RUIN::RenderArea RUIN::HorizontalBox::GetAreaForChild(const Erm::vec2f& availableArea, const RenderArea& usedArea, const RenderContext& ctx) const
{
	RenderArea childArea{};
	childArea.w = availableArea.w;
	childArea.h = availableArea.h;

	if (!m_Flex)
	{
		const float width = availableArea.x / GetNumChildren();
		childArea.w = width;
		childArea.x += width * ctx.childIndex;
	}
	else
	{
		childArea.x = usedArea.w;
		childArea.w -= usedArea.w;
	}

	return childArea;
}
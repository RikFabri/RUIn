#include "VerticalBox.h"

RUIN::VerticalBox::VerticalBox(tinyxml2::XMLElement* e)
	: UIContainer(e)
	, m_Flex(false)
{
	e->QueryBoolAttribute("isFlexible", &m_Flex);
}

RUIN::RenderArea RUIN::VerticalBox::GetAreaForChild(const Erm::vec2f& availableArea, const RenderArea& usedArea, const RenderContext& ctx) const
{
	RenderArea childArea{};
	childArea.w = availableArea.w;
	childArea.h = availableArea.h;

	if (!m_Flex)
	{
		const float height = availableArea.y / GetNumChildren();
		childArea.h = height;
		childArea.y += height * ctx.childIndex;
	}
	else
	{
		childArea.y = usedArea.h;
		childArea.h -= usedArea.h;
	}

	return childArea;
}
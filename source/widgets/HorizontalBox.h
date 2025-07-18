#pragma once
#include "../layouting/Container.h"

namespace RUIN
{
	class HorizontalBox : public UIContainer
	{
	public:
		HorizontalBox(tinyxml2::XMLElement* e);
	private:
		virtual RenderArea GetAreaForChild(const RenderArea& availableArea, RenderArea& usedArea, RenderContext& ctx) const override;
		bool m_Flex = false;
	};
}
#pragma once
#include "../layouting/Container.h"

namespace RUIN
{
	class VerticalBox : public UIContainer
	{
	public:
		VerticalBox(tinyxml2::XMLElement* e);
		virtual RenderArea GetAreaForChild(const RenderArea& availableArea, RenderArea& usedArea, RenderContext& ctx) const override;

	private:
		bool m_Flex = false;
	};
}
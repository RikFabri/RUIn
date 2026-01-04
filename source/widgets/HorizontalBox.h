#pragma once
#include "../layouting/Container.h"

namespace RUIN
{
	class HorizontalBox : public UIContainer
	{
	public:
		HorizontalBox(tinyxml2::XMLElement* e);
	private:
		virtual RUIN::RenderArea GetAreaForChild(const RenderArea& availableArea, const RenderArea& usedArea, const RenderContext& ctx) const override;
		bool m_Flex = false;
	};
}
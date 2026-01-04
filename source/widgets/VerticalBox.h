#pragma once
#include "../layouting/Container.h"

namespace RUIN
{
	class VerticalBox : public UIContainer
	{
	public:
		VerticalBox(tinyxml2::XMLElement* e);
		virtual RUIN::RenderArea GetAreaForChild(const Erm::vec2f& availableArea, const RenderArea& usedArea, const RenderContext& ctx) const override;

	private:
		bool m_Flex = false;
	};
}
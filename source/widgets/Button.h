#pragma once
#include "../layouting/LeafNode.h"
#include "../UIManager.h"
#include "tinyxml2.h"
#include <string>
#include <iostream>

namespace RUIN
{
	class Button : public UIContainer
	{
	public:
		Button(tinyxml2::XMLElement* element);
		void Render(const RenderArea& targetArea) override;

	private:

		RenderArea GetAreaForChild(const Erm::vec2f& availableArea, const RenderArea& usedArea, const RenderContext& ctx) const override;
		bool PropagateEvent(const Erm::vec2f& position, int cursorX, int cursorY, const Event& event) override;

		enum class ButtonState
		{
			Default,
			Hovered,
			Down,
		};
		
		ButtonState m_ButtonState = ButtonState::Default;

		std::string m_OnClick;
	};

}
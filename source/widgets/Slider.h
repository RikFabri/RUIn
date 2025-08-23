#pragma once
#include "../layouting/LeafNode.h"
#include "../UIManager.h"

namespace RUIN
{
	class Slider : public LeafNode
	{
	public:
		Slider(tinyxml2::XMLElement* element);

	private:
		void Render(const RenderArea& targetArea) override;
		Erm::vec2f GetDimensions() override;
		bool HandleMouseMoved(int cursorX, int cursorY) override;
		bool HandleMouseDown(int cursorX, int cursorY) override;
		bool HandleMouseUp(int cursorX, int cursorY) override;

		float ValueToPercentage(float val) const;
		float OffsetToValue(float offset) const;

		void OnTextChanged();

		float m_Value = 0.f;
		float m_MinValue = 0.f;
		float m_MaxValue = 10.f;
		float m_Precision = 1.f;

		ClientTexture m_pTextImage;

		Erm::vec2i m_StartPos;
		bool m_Editing = false;
	};

}
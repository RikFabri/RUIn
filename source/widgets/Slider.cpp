#include "Slider.h"
#include "databinding/DataBinding.h"

#include <algorithm>


RUIN::Slider::Slider(tinyxml2::XMLElement* element)
	: LeafNode(element)
{
	Bind_member_to_XML(m_Value, element, "value").OnChange(OnTextChanged);

	Bind_member_to_XML(m_MinValue, element, "minValue");
	Bind_member_to_XML(m_MaxValue, element, "maxValue");
	Bind_member_to_XML(m_Precision, element, "precision");

}

void RUIN::Slider::Render(const RenderArea& targetArea)
{
	UIManager::GetInstance().DrawRectangle(targetArea, { 39, 49, 54, 255 });
	RenderArea textArea = targetArea;
	textArea.x += 2.f;
	textArea.y += 2.f;
	m_pTextImage.Draw(textArea);

	const auto percentage = ValueToPercentage(m_Value);
	const auto position = percentage * targetArea.w + targetArea.x;

	RenderArea lineArea = targetArea;
	lineArea.x = position;
	lineArea.w = 3.f;

	UIManager::GetInstance().DrawRectangle(lineArea, { 211, 218, 200, 255 });

}

Erm::vec2f RUIN::Slider::GetDimensions()
{
	uint32_t w, h;
	m_pTextImage.GetDimensions(w, h);

	return Erm::vec2f((float)w + 4.f, (float)h + 4.f);
}

bool RUIN::Slider::HandleMouseMoved(int cursorX, int )
{
	if (!m_Editing)
	{
		return false;
	}

	// TODO: working with this offset is quite bad, can we provide widgets their renderArea in a clean way?
	const auto offset = cursorX - m_StartPos.x;
	m_Value = OffsetToValue((float)offset);
	OnTextChanged();

	Notify_member_changed(m_Value);

	return true;
}

bool RUIN::Slider::HandleMouseDown(int cursorX, int cursorY)
{
	m_StartPos.x = cursorX;
	m_StartPos.y = cursorY;
	m_Editing = true;

	return true;
}

bool RUIN::Slider::HandleMouseUp(int, int)
{
	m_Editing = false;
	return true;
}

float RUIN::Slider::ValueToPercentage(float val) const
{
	const auto differenceToMin = val - m_MinValue;
	const auto range = m_MaxValue - m_MinValue;
	const auto percentage = differenceToMin / range;

	return percentage;
}

float RUIN::Slider::OffsetToValue(float offset) const
{
	offset *= m_Precision * 0.01f;

	uint32_t w, h;
	m_pTextImage.GetDimensions(w, h);

	const auto range = m_MaxValue - m_MinValue;
	const auto percentage = offset / (float)w;

	const auto result = std::clamp(m_Value + range * percentage, m_MinValue, m_MaxValue);
	return result;
}

void RUIN::Slider::OnTextChanged()
{
	const auto text = std::to_string(m_Value);
	m_pTextImage = UIManager::GetInstance().AllocateTextureFromText(text);
}

#include "Label.h"
#include "../UIManager.h"
#include "databinding/DataBinding.h"


RUIN::Label::Label(tinyxml2::XMLElement* element)
	: LeafNode(element)
{
	Bind_member_to_XML(m_Text, element, "Text").OnChange(OnTextChanged);
}

void RUIN::Label::Render(const RenderArea& targetArea)
{
	UIManager::GetInstance().DrawRectangle(targetArea, { 125, 125, 0, 255 });
	UIManager::GetInstance().DrawTexture(m_pTextImage, targetArea);
}

void RUIN::Label::OnTextChanged()
{
	m_pTextImage = UIManager::GetInstance().AllocateTextureFromText(m_Text);
}

Erm::vec2f RUIN::Label::GetDimensions()
{
	uint32_t w, h;
	UIManager::GetInstance().QueryTextureDimensions(m_pTextImage, w, h);
	return Erm::vec2f((float)w, (float)h);
}